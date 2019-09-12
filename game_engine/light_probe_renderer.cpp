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
	static int j{};
	for (size_t i{}; i < light_probes.size(); i++)
	{
		if (!j)
		{
			j = 1;
			create_radiance_cubemap(i);
		}
		process_radiance_map(i);
	}
}

void aech::graphics::light_probe_renderer_t::create_radiance_cubemap(size_t probe_index)
{
	const static auto capture_projection = math::perspective(90, 1, 0.1f, 1500.0f);

	auto& probe = light_probes[probe_index];
	const std::array capture_views
	{
		math::look_at(probe.position, probe.position+ math::vec3_t{1, 0, 0}, {0, 1, 0}),
		math::look_at(probe.position, probe.position+ math::vec3_t{-1, 0, 0}, {0, 1, 0}),		
		math::look_at(probe.position, probe.position + math::vec3_t{0, -1, 0}, {0, 0, -1}),
		math::look_at(probe.position, probe.position + math::vec3_t{0, 1, 0}, {0, 0, 1}),

		math::look_at(probe.position, probe.position + math::vec3_t{0, 0, -1}, {0, 1, 0}),
		math::look_at(probe.position, probe.position + math::vec3_t{0, 0, 1}, {0, 1, 0})
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
		cubemap_capture_material->m_shader->use();
		cubemap_capture_material->m_shader->set_uniform("view", view);
		cubemap_capture_material->m_shader->set_uniform("projection", capture_projection);
		for (auto entity : entities)
		{
			auto& transf = engine.get_component<transform_t>(entity);
			auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
			for (auto texture: mesh_filter.material->m_textures)
			{
				cubemap_capture_material->set_texture(texture.first, texture.second.first, texture.second.second);
			}

			cubemap_capture_material->m_shader->set_uniform("model", transf.get_transform_matrix());
			cubemap_capture_material->set_uniforms();

			glBindVertexArray(mesh_filter.mesh->m_vao);
			glDrawElements(GL_TRIANGLES, mesh_filter.mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
		}
	}

	fbo.unbind();
}

void graphics::light_probe_renderer_t::process_radiance_map(size_t probe_index)
{
	// diffuse precomputation
	const static auto capture_projection = math::perspective(90, 1, 0.1f, 1500.0f);
	
	auto& probe = light_probes[probe_index];
	const std::array capture_views
	{
		math::look_at(probe.position, probe.position + math::vec3_t{1, 0, 0}, {0, 1, 0}),
		math::look_at(probe.position, probe.position + math::vec3_t{-1, 0, 0}, {0, 1, 0}),
		math::look_at(probe.position, probe.position + math::vec3_t{0, -1, 0}, {0, 0, -1}),
		math::look_at(probe.position, probe.position + math::vec3_t{0, 1, 0}, {0, 0, 1}),

		math::look_at(probe.position, probe.position + math::vec3_t{0, 0, -1}, {0, 1, 0}),
		math::look_at(probe.position, probe.position + math::vec3_t{0, 0, 1}, {0, 1, 0})
	};

	probe.irradiance = &resource_manager::texture_cubes["irradiance" + std::to_string(probe_index)];
	probe.irradiance->width = 32;
	probe.irradiance->height = 32;
	probe.irradiance->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	probe.irradiance->format = texture_types::format::rgb;
	probe.irradiance->type = texture_types::type::floating_point;
	probe.irradiance->init();

	irradiance_capture_material->m_shader->use();
	irradiance_capture_material->set_texture_cube("environment", &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)], 0);

	framebuffer_cube_t fbo{ &resource_manager::texture_cubes["irradiance" + std::to_string(probe_index)], 32, 32 };

	fbo.bind();
	glViewport(0, 0, fbo.width, fbo.height);

	for (size_t i = 0; i < 6; i++)
	{
		fbo.attach(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		irradiance_capture_material->m_shader->set_uniform("projection", capture_projection);
		irradiance_capture_material->m_shader->set_uniform("view", capture_views[i]);			irradiance_capture_material->set_uniforms();
		glBindVertexArray(ndc_cube->m_vao);
		glDrawArrays(GL_TRIANGLES, 0, ndc_cube->m_positions.size());
	}

	fbo.unbind();

	// specular precomputation
	probe.prefiltered = &resource_manager::texture_cubes["prefiltered" + std::to_string(probe_index)];
	probe.prefiltered->width = 128;
	probe.prefiltered->height = 128;
	probe.prefiltered->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	probe.prefiltered->format = texture_types::format::rgb;
	probe.prefiltered->type = texture_types::type::floating_point;
	probe.prefiltered->init();

	framebuffer_cube_t prefilter_fbo{ &resource_manager::texture_cubes["prefiltered" + std::to_string(probe_index)], 128, 128 };
	prefilter_fbo.bind();

	prefilter_material->m_shader->use();
	prefilter_material->set_texture_cube("environment_map", &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)], 0);
	prefilter_material->set_uniform("projection", capture_projection);

	if (glGetError()) std::clog << "ERROR" << std::endl;
	const int levels = floor(log2(std::max(128, 128))) + 1;
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
			if (glGetError()) std::clog << "ERROR" << std::endl;

			if (glGetError()) std::clog << "ERROR" << std::endl;

			glBindVertexArray(ndc_cube->m_vao);
			glDrawArrays(GL_TRIANGLES, 0, ndc_cube->m_positions.size());
		}
	}


	prefilter_fbo.unbind();
}
