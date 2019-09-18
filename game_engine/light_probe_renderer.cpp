#include "light_probe_renderer.hpp"
#include "framebuffer_cube.hpp"

#include "framebuffer_library.hpp"

#include "main.hpp"

#include "mat4.hpp"

#include "mesh_filter.hpp"
#include "transforms.hpp"

#include <array>

#include <iomanip>

#include <iostream>


void graphics::light_probe_renderer_t::bake_probes()
{
	std::clog << "Baking light probes" << std::endl;
	for (size_t i{}; i < light_probes.size(); i++)
	{
		std::clog << std::setw(6) << std::fixed << std::setprecision(2) << static_cast<float>(i) / light_probes.size() *
			100 << "%" << std::endl;
		create_radiance_cubemap(i);
		process_radiance_map(i);
	}
	std::clog << "100.00%" << std::endl;
}

void graphics::light_probe_renderer_t::create_radiance_cubemap(size_t probe_index)
{
	const static auto capture_projection = math::perspective(90, 1, 0.1f, 4000.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	auto& probe = light_probes[probe_index];

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
	tex->width                 = 1024;
	tex->height                = 1024;
	tex->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	tex->format                = texture_types::format::rgb;
	tex->type                  = texture_types::type::floating_point;
	tex->init();

	framebuffer_cube_t fbo{tex, 1024, 1024};
	fbo.bind();
	glViewport(0, 0, fbo.width, fbo.height);
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
	fbo.texture->generate_mips();
	fbo.unbind();
}

void graphics::light_probe_renderer_t::process_radiance_map(size_t probe_index)
{
	glDisable(GL_CULL_FACE);
	const static auto capture_projection = math::perspective(90, 1, 0.1F, 10.0F);

	auto& probe = light_probes[probe_index];
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
	irradiance->width                 = 32;
	irradiance->height                = 32;
	irradiance->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	irradiance->format                = texture_types::format::rgb;
	irradiance->type                  = texture_types::type::floating_point;
	irradiance->init();

	probe.set_irradiance(irradiance);

	irradiance_capture_material->shader()->use();
	irradiance_capture_material->set_texture_cube("environment",
	                                              &resource_manager::texture_cubes[
		                                              "radiance" + std::to_string(probe_index)],
	                                              0);

	framebuffer_cube_t fbo{probe.irradiance(), 32, 32};

	fbo.bind();
	glViewport(0, 0, fbo.width, fbo.height);

	for (size_t i = 0; i < 6; i++)
	{
		fbo.attach(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		irradiance_capture_material->shader()->set_uniform("projection", capture_projection);
		irradiance_capture_material->shader()->set_uniform("view", capture_views[i]);
		irradiance_capture_material->set_uniforms();
		ndc_cube->draw();
	}
	fbo.texture->generate_mips();
	fbo.unbind();

	// specular precomputation

	// precompute prefiltered env map
	auto prefiltered = &resource_manager::texture_cubes["prefiltered" + std::to_string(probe_index)];
	prefiltered->width                 = 128;
	prefiltered->height                = 128;
	prefiltered->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	prefiltered->format                = texture_types::format::rgb;
	prefiltered->type                  = texture_types::type::floating_point;
	prefiltered->init();

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
	glDisable(GL_DEPTH_TEST);

	auto& brdf_fbo = framebuffers["brdf"];
	glViewport(0, 0, brdf_fbo.width(), brdf_fbo.height());
	brdf_material->shader()->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ndc_quad->draw();
	brdf_fbo.unbind();
	glEnable(GL_CULL_FACE);
}


void graphics::light_probe_renderer_t::render_ambient_pass()
{
	render_target->bind();
	glViewport(0, 0, render_target->width(), render_target->height());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	ambient_material->shader()->use();
	ambient_material->set_texture("brdf_lut", &framebuffers["brdf"].colour_attachments().front(), 6);
	ambient_material->set_uniform("camera_position", camera_transform->position);
	ambient_material->set_uniform("projection", camera->projection);
	ambient_material->set_uniform("view", math::get_view_matrix(*camera_transform));
	ambient_material->set_uniform("camera_position", camera_transform->position);
	for (auto& probe : light_probes)
	{
		ambient_material->set_uniform("probe_position", probe.position());
		ambient_material->set_texture_cube("environment_irradiance", probe.irradiance(), 7);
		ambient_material->set_texture_cube("environment_prefiltered", probe.prefiltered(), 8);
		ambient_material->set_uniform("model", translate(probe.position()) * math::scale(probe.radius()));
		ambient_material->set_uniform("probe_radius", probe.radius());
		ambient_material->set_uniforms();
		ndc_sphere->draw();
	}
}
