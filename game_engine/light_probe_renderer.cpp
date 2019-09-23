#include "light_probe_renderer.hpp"
#include "framebuffer_cube.hpp"

#include "framebuffer_library.hpp"



#include "mat4.hpp"

#include "mesh_filter.hpp"
#include "transforms.hpp"

#include <array>

#include <iomanip>

#include <iostream>
#include "main.hpp"


void aech::graphics::light_probe_renderer_t::set_camera(camera_t* camera)
{
	m_camera = camera;
}

void aech::graphics::light_probe_renderer_t::set_camera_transform(transform_t* transform)
{
	m_camera_transform = transform;
}

aech::graphics::material_t* aech::graphics::light_probe_renderer_t::ambient_material() const
{
	return m_ambient_material;
}


::aech::graphics::framebuffer_t* ::aech::graphics::light_probe_renderer_t::render_target() const
{
	return m_render_target;
}

void ::aech::graphics::light_probe_renderer_t::add_probe(light_probe_t&& probe)
{
	m_light_probes.push_back(probe);
}


void aech::graphics::light_probe_renderer_t::add_probe(const light_probe_t& probe)
{
		m_light_probes.push_back(probe);
}

void aech::graphics::light_probe_renderer_t::bake_probes()
{
	std::clog << "Rendering brdf LUT" << std::endl;
	auto& brdf_fbo = framebuffers["brdf"];
	brdf_fbo.bind();
	glViewport(0, 0, brdf_fbo.width(), brdf_fbo.height());
	brdf_material->shader()->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ndc_quad->draw();
	brdf_fbo.unbind();
	
	std::clog << "Baking light probes" << std::endl;
	for (size_t i{}; i < m_light_probes.size(); i++)
	{
		std::clog << std::setw(6) << std::fixed << std::setprecision(2) << static_cast<float>(i) / m_light_probes.size() *
			100 << "%" << std::endl;
		create_radiance_cubemap(i);
		process_radiance_map(i);
	}
	
	
	std::clog << "100.00%" << std::endl;
}

void aech::graphics::light_probe_renderer_t::create_radiance_cubemap(size_t probe_index)
{
	const static auto capture_projection = math::perspective(90, 1, 0.1f, 4000.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	auto& probe = m_light_probes[probe_index];

	const std::array capture_views
	{
		look_at(probe.position(), probe.position() + math::vec3_t{1, 0, 0}, {0, -1, 0}),
		look_at(probe.position(), probe.position() + math::vec3_t{-1, 0, 0}, {0, -1, 0}),
		look_at(probe.position(), probe.position() + math::vec3_t{0, 1, 0}, {0, 0, 1}),
		look_at(probe.position(), probe.position() + math::vec3_t{0, -1, 0}, {0, 0, -1}),

		look_at(probe.position(), probe.position() + math::vec3_t{0, 0, 1}, {0, -1, 0}),
		look_at(probe.position(), probe.position() + math::vec3_t{0, 0, -1}, {0, -1, 0})
	};

	auto tex                   = &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)];
	*tex = texture_cube_t{1024, 1024, texture_types::sized_internal_format::rgb32f, texture_types::format::rgb, texture_types::type::floating_point};
	
	framebuffer_cube_t fbo{tex, 1024, 1024};
	fbo.bind();
	glViewport(0, 0, fbo.width(), fbo.height());
	for (uint32_t i = 0; i < 6; i++)
	{
		fbo.attach(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		auto& view = capture_views[i];
		cubemap_capture_material->shader()->use();
		cubemap_capture_material->set_uniform("view", view);
		cubemap_capture_material->set_uniform("projection", capture_projection);
		for (auto entity : entities)
		{
			auto& transf      = engine.get_component<transform_t>(entity);
			auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
			for (const auto &texture : mesh_filter.material()->get_textures())
			{
				cubemap_capture_material->set_texture(texture.first, texture.second.first, texture.second.second);
			}

			cubemap_capture_material->shader()->use();
			cubemap_capture_material->shader()->set_uniform("model", transf.get_transform_matrix());
			cubemap_capture_material->set_uniforms();
			mesh_filter.mesh()->draw();
		}

		// render skybox
		glDisable(GL_CULL_FACE);
		cubemap_capture_skybox_material->shader()->use();
		cubemap_capture_skybox_material->set_uniform("view", view);
		cubemap_capture_skybox_material->set_uniform("projection", capture_projection);
		cubemap_capture_skybox_material->
			set_texture_cube("skybox", skybox_mf.material()->get_texture_cube("skybox"), 0);
		cubemap_capture_skybox_material->set_uniforms();
		skybox_mf.mesh()->draw();
		glEnable(GL_CULL_FACE);
	}
	fbo.texture()->generate_mips();
			
	fbo.unbind();
}

void aech::graphics::light_probe_renderer_t::process_radiance_map(size_t probe_index)
{
	glDisable(GL_CULL_FACE);
	const static auto capture_projection = math::perspective(90, 1, 0.1F, 10.0F);

	auto& probe = m_light_probes[probe_index];
	const std::array                       capture_views
	{
		look_at({}, math::vec3_t{1, 0, 0}, {0, -1, 0}),
		look_at({}, math::vec3_t{-1, 0, 0}, {0, -1, 0}),
		look_at({}, math::vec3_t{0, 1, 0}, {0, 0, 1}),
		look_at({}, math::vec3_t{0, -1, 0}, {0, 0, -1}),

		look_at({}, math::vec3_t{0, 0, 1}, {0, -1, 0}),
		look_at({}, math::vec3_t{0, 0, -1}, {0, -1, 0})
	};
	// diffuse precomputation

	auto irradiance                   = &resource_manager::texture_cubes["irradiance" + std::to_string(probe_index)];
	*irradiance = texture_cube_t {32, 32, texture_types::sized_internal_format::rgb32f, texture_types::format::rgb, texture_types::type::floating_point};

	probe.set_irradiance(irradiance);

	irradiance_capture_material->shader()->use();
	irradiance_capture_material->set_texture_cube("environment",
	                                              &resource_manager::texture_cubes[
		                                              "radiance" + std::to_string(probe_index)],
	                                              0);

	framebuffer_cube_t fbo{probe.irradiance(), 32, 32};

	fbo.bind();
	glViewport(0, 0, fbo.width(), fbo.height());

	for (size_t i = 0; i < 6; i++)
	{
		fbo.attach(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		irradiance_capture_material->shader()->set_uniform("projection", capture_projection);
		irradiance_capture_material->shader()->set_uniform("view", capture_views[i]);
		irradiance_capture_material->set_uniforms();
		ndc_cube->draw();
	}
	fbo.texture()->generate_mips();
	fbo.unbind();

	// specular precomputation

	// precompute prefiltered env map
	auto prefiltered = &resource_manager::texture_cubes["prefiltered" + std::to_string(probe_index)];
	*prefiltered = texture_cube_t {128, 128, texture_types::sized_internal_format::rgb32f, texture_types::format::rgb, texture_types::type::floating_point};

	probe.set_prefiltered(prefiltered);

	framebuffer_cube_t prefilter_fbo{probe.prefiltered(), 128, 128};
	prefilter_fbo.bind();

	prefilter_material->shader()->use();
	prefilter_material->set_texture_cube("environment_map",
	                                     &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)],
	                                     0);
	prefilter_material->set_uniform("projection", capture_projection);

	const int levels = floor(log2(128)) + 1;
	for (uint32_t mip = 0; mip < levels; mip++)
	{
		uint32_t width  = 128 * std::pow(0.5, mip);
		auto     height = width;
		glViewport(0, 0, width, height);
		auto roughness = static_cast<float>(mip) / (levels - 1);
		prefilter_material->set_uniform("roughness", roughness);
		for (uint32_t i = 0; i < 6; i++)
		{
			prefilter_material->set_uniform("view", capture_views[i]);
			prefilter_material->set_uniforms();
			prefilter_fbo.attach(i, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ndc_cube->draw();
		}
	}
	prefilter_fbo.unbind();
}


void aech::graphics::light_probe_renderer_t::render_ambient_pass()
{
	m_render_target->bind();
	glViewport(0, 0, m_render_target->width(), m_render_target->height());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	m_ambient_material->shader()->use();
	m_ambient_material->set_texture("brdf_lut", &framebuffers["brdf"].colour_attachments().front(), 6);
	m_ambient_material->set_uniform("camera_position", m_camera_transform->position);
	m_ambient_material->set_uniform("projection", m_camera->projection());
	m_ambient_material->set_uniform("view", math::get_view_matrix(*m_camera_transform));
	m_ambient_material->set_uniform("camera_position", m_camera_transform->position);
	for (auto& probe : m_light_probes)
	{
		// view frustum culling of probes
		if (!m_camera->sees(probe.position(), probe.radius())) continue;
		m_ambient_material->set_uniform("probe_position", probe.position());
		m_ambient_material->set_texture_cube("environment_irradiance", probe.irradiance(), 7);
		m_ambient_material->set_texture_cube("environment_prefiltered", probe.prefiltered(), 8);
		m_ambient_material->set_uniform("model", translate(probe.position()) * math::scale(probe.radius()));
		m_ambient_material->set_uniform("probe_radius", probe.radius());
		m_ambient_material->set_uniforms();
		ndc_sphere->draw();
	}
}
