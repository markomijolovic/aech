#include "light_probe_renderer.hpp"
#include "transforms.hpp"
#include "mat4.hpp"
#include <array>
#include "main.hpp"
#include "mesh_filter.hpp"
#include "framebuffer_cube.hpp"
#include <iostream>
#include "framebuffer_library.hpp"

void aech::graphics::light_probe_renderer_t::bake_probes()
{
	std::clog << "Baking light probes" << std::endl;
	// remov this LUL
	for (size_t i{}; i < light_probes.size(); i++)
	{
		create_radiance_cubemap(i);
		process_radiance_map(i);
	}
}

void aech::graphics::light_probe_renderer_t::create_radiance_cubemap(size_t probe_index)
{
	const static auto capture_projection = math::perspective(90, 1, 0.1f, 4000.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	auto& probe = light_probes[probe_index];

	const std::array capture_views
	{
		math::look_at(probe.position, probe.position + math::vec3_t{1, 0, 0}, {0, -1, 0}),
		math::look_at(probe.position, probe.position + math::vec3_t{-1, 0, 0}, {0, -1, 0}),
		math::look_at(probe.position, probe.position + math::vec3_t{0, 1, 0}, {0, 0, 1}),
		math::look_at(probe.position, probe.position + math::vec3_t{0, -1, 0}, {0, 0, -1}),

		math::look_at(probe.position, probe.position + math::vec3_t{0, 0, 1}, {0, -1, 0}),
		math::look_at(probe.position, probe.position + math::vec3_t{0, 0, -1}, {0, -1, 0})
	};

	auto tex = &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)];
	tex->width = 1024;
	tex->height = 1024;
	tex->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	tex->format = texture_types::format::rgb;
	tex->type = texture_types::type::floating_point;
	tex->init();

	framebuffer_cube_t fbo{ tex, 1024,1024 };
	fbo.bind();
	glViewport(0, 0, fbo.width, fbo.height);
	for (uint32_t i = 0; i < 6; i++)
	{
		fbo.attach(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		auto& view = capture_views[i];
		cubemap_capture_material->set_uniform("view", view);
		cubemap_capture_material->set_uniform("projection", capture_projection);
		cubemap_capture_skybox_material->m_shader->use();
		cubemap_capture_skybox_material->set_uniform("view", view);
		cubemap_capture_skybox_material->set_uniform("projection", capture_projection);
		for (auto entity : entities)
		{
			auto& transf = engine.get_component<transform_t>(entity);
			auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
			for (auto texture : mesh_filter.material->m_textures)
			{
				cubemap_capture_material->m_shader->use();
				cubemap_capture_material->set_texture(texture.first, texture.second.first, texture.second.second);
			}
			glBindVertexArray(mesh_filter.mesh->m_vao);
			if (mesh_filter.material->m_texture_cubes.find("skybox") != mesh_filter.material->m_texture_cubes.end())
			{
				glDisable(GL_CULL_FACE);
				cubemap_capture_skybox_material->m_shader->use();
				cubemap_capture_skybox_material->set_texture_cube("skybox", &resource_manager::texture_cubes["skybox"], 0);
				cubemap_capture_skybox_material->set_uniforms();
				glDrawArrays(GL_TRIANGLES, 0, mesh_filter.mesh->m_positions.size());
				glEnable(GL_CULL_FACE);
			}
			else
			{
				cubemap_capture_material->m_shader->use();
				cubemap_capture_material->m_shader->set_uniform("model", transf.get_transform_matrix());
				cubemap_capture_material->set_uniforms();
				glDrawElements(GL_TRIANGLES, mesh_filter.mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
			}
		}
	}
	fbo.texture->generate_mips();
	fbo.unbind();
}

void graphics::light_probe_renderer_t::process_radiance_map(size_t probe_index)
{
	glDisable(GL_CULL_FACE);
	const static auto capture_projection = math::perspective(90, 1, 0.1f, 10.0f);
	
	auto& probe = light_probes[probe_index];
	const std::array capture_views
	{
		math::look_at({}, math::vec3_t{1, 0, 0}, {0, -1, 0}),
		math::look_at({}, math::vec3_t{-1, 0, 0}, {0, -1, 0}),
		math::look_at({},  math::vec3_t{0, 1, 0}, {0, 0, 1}),
		math::look_at({}, math::vec3_t{0, -1, 0}, {0, 0, -1}),

		math::look_at({},  math::vec3_t{0, 0, 1}, {0, -1, 0}),
		math::look_at({}, math::vec3_t{0, 0, -1}, {0, -1, 0})
	};
	// diffuse precomputation

	probe.irradiance = &resource_manager::texture_cubes["irradiance" + std::to_string(probe_index)];
	probe.irradiance->width = 32;
	probe.irradiance->height = 32;
	probe.irradiance->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	probe.irradiance->format = texture_types::format::rgb;
	probe.irradiance->type = texture_types::type::floating_point;
	probe.irradiance->init();

	irradiance_capture_material->m_shader->use();
	irradiance_capture_material->set_texture_cube("environment", &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)], 0);

	framebuffer_cube_t fbo{ probe.irradiance, 32, 32 };

	fbo.bind();
	glViewport(0, 0, fbo.width, fbo.height);

	for (size_t i = 0; i < 6; i++)
	{
		fbo.attach(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		irradiance_capture_material->m_shader->set_uniform("projection", capture_projection);
		irradiance_capture_material->m_shader->set_uniform("view", capture_views[i]);			
		irradiance_capture_material->set_uniforms();
		glBindVertexArray(ndc_cube->m_vao);
		glDrawArrays(GL_TRIANGLES, 0, ndc_cube->m_positions.size());
	}
	fbo.texture->generate_mips();
	fbo.unbind();

	// specular precomputation

	// precompute prefiltered env map
	probe.prefiltered = &resource_manager::texture_cubes["prefiltered" + std::to_string(probe_index)];
	probe.prefiltered->width = 128;
	probe.prefiltered->height = 128;
	probe.prefiltered->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	probe.prefiltered->format = texture_types::format::rgb;
	probe.prefiltered->type = texture_types::type::floating_point;
	probe.prefiltered->init();

	framebuffer_cube_t prefilter_fbo{ probe.prefiltered, 128, 128 };
	prefilter_fbo.bind();

	prefilter_material->m_shader->use();
	prefilter_material->set_texture_cube("environment_map", &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)], 0);
	prefilter_material->set_uniform("projection", capture_projection);

	const int levels = floor(log2(128)) + 1;
	for (uint32_t mip = 0; mip < levels; mip++)
	{
		uint32_t width = 128 * std::pow(0.5, mip);
		auto height = width;
		glViewport(0, 0, width, height);
		auto roughness = static_cast<float>(mip) / (levels - 1);
		prefilter_material->set_uniform("roughness", roughness);
		for (uint32_t i = 0; i < 6; i++)
		{
			prefilter_material->set_uniform("view", capture_views[i]);
			prefilter_material->set_uniforms();
			prefilter_fbo.attach(i, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(ndc_cube->m_vao);
			glDrawArrays(GL_TRIANGLES, 0, ndc_cube->m_positions.size());
		}
	}

	prefilter_fbo.unbind();
	glDisable(GL_DEPTH_TEST);

	auto& brdf_fbo = framebuffers["brdf"];
	brdf_fbo.bind();
	glViewport(0, 0, brdf_fbo.width, brdf_fbo.height);
	brdf_material->m_shader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(ndc_quad->m_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, ndc_quad->m_positions.size());
	brdf_fbo.unbind();
	glEnable(GL_CULL_FACE);
}


void graphics::light_probe_renderer_t::render_ambient_pass()
{
	render_target->bind();
	glViewport(0, 0, render_target->width, render_target->height);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	ambient_material->m_shader->use();
	ambient_material->set_texture("brdf_lut", &framebuffers["brdf"].m_colour_attachments.front(), 6);
	ambient_material->set_uniform("camera_position", camera_transform->position);
	ambient_material->set_uniform("projection", camera->projection);
	ambient_material->set_uniform("view", math::get_view_matrix(*camera_transform));
	ambient_material->set_uniform("camera_position", camera_transform->position);
	for (auto &probe: light_probes)
	{
		ambient_material->set_uniform("probe_position", probe.position);
		ambient_material->set_texture_cube("environment_irradiance", probe.irradiance, 7);
		ambient_material->set_texture_cube("environment_prefiltered", probe.prefiltered, 8);
		ambient_material->set_uniform("model", translate(probe.position) * math::scale(probe.radius));
		ambient_material->set_uniform("probe_radius", probe.radius);
		ambient_material->set_uniforms();
		glBindVertexArray(ndc_sphere->m_vao);
		glDrawElements(GL_TRIANGLES, ndc_sphere->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	glEnable(GL_CULL_FACE);
}
