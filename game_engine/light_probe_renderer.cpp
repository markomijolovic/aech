#include "light_probe_renderer.hpp"
#include "transforms.hpp"
#include "mat4.hpp"
#include <array>
#include "main.hpp"
#include "mesh_filter.hpp"
#include "framebuffer_cube.hpp"
#include <iostream>

void aech::graphics::light_probe_renderer_t::bake_probes()
{
	std::clog << "Baking light probes" << std::endl;
	for (auto &light_probe: light_probes){
		create_radiance_cubemap(light_probe.position);
		process_radiance_map(light_probe.position);
	}
}

void aech::graphics::light_probe_renderer_t::create_radiance_cubemap(math::vec3_t& eye)
{
	const static auto capture_projection = math::perspective(90, 1, 0.1f, 1500.0f);

	const std::array capture_views
	{
		math::look_at(eye, eye+ math::vec3_t{1, 0, 0}, {0, 1, 0}),
		math::look_at(eye, eye+ math::vec3_t{-1, 0, 0}, {0, 1, 0}),		
		math::look_at(eye, eye + math::vec3_t{0, -1, 0}, {0, 0, -1}),
		math::look_at(eye, eye + math::vec3_t{0, 1, 0}, {0, 0, 1}),

		math::look_at(eye, eye + math::vec3_t{0, 0, -1}, {0, 1, 0}),
		math::look_at(eye, eye + math::vec3_t{0, 0, 1}, {0, 1, 0})
	};

	auto tex = &resource_manager::texture_cubes["radiance" + std::to_string(light_probes.size())];
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

void graphics::light_probe_renderer_t::process_radiance_map(const math::vec3_t &eye)
{
	const static auto capture_projection = math::perspective(90, 1, 0.1f, 1500.0f);

	const std::array capture_views
	{
		math::look_at(eye, eye + math::vec3_t{1, 0, 0}, {0, 1, 0}),
		math::look_at(eye, eye + math::vec3_t{-1, 0, 0}, {0, 1, 0}),
		math::look_at(eye, eye + math::vec3_t{0, -1, 0}, {0, 0, -1}),
		math::look_at(eye, eye + math::vec3_t{0, 1, 0}, {0, 0, 1}),

		math::look_at(eye, eye + math::vec3_t{0, 0, -1}, {0, 1, 0}),
		math::look_at(eye, eye + math::vec3_t{0, 0, 1}, {0, 1, 0})
	};

	auto& probe = light_probes.back();
	probe.irradiance = &resource_manager::texture_cubes["irradiance" + std::to_string(light_probes.size())];
	probe.irradiance->width = 32;
	probe.irradiance->height = 32;
	probe.irradiance->sized_internal_format = texture_types::sized_internal_format::rgb32f;
	probe.irradiance->format = texture_types::format::rgb;
	probe.irradiance->type = texture_types::type::floating_point;
	probe.irradiance->init();

	irradiance_capture_material->m_shader->use();
	irradiance_capture_material->set_texture_cube("environment", &resource_manager::texture_cubes["radiance" + std::to_string(light_probes.size())], 0);

	framebuffer_cube_t fbo{ &resource_manager::texture_cubes["irradiance" + std::to_string(light_probes.size())], 32, 32 };

	fbo.bind();
	glViewport(0, 0, fbo.width, fbo.height);

	for (size_t i = 0; i < 6; i++)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbo.attach(i);
		irradiance_capture_material->m_shader->set_uniform("projection", capture_projection);
		irradiance_capture_material->m_shader->set_uniform("view", capture_views[i]);			irradiance_capture_material->set_uniforms();
		glBindVertexArray(ndc_cube->m_vao);
		glDrawArrays(GL_TRIANGLES, 0, ndc_cube->m_positions.size());
	}

	fbo.unbind();
}
