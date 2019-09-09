#include "renderer.hpp"
#include "mesh_library.hpp"
#include "material_library.hpp"
#include "camera.hpp"
#include "directional_light.hpp"
#include "point_light.hpp"
#include "shadow_caster.hpp"
#include "shading_tags.hpp"
#include "transparent_shadow_renderer.hpp"
#include "transforms.hpp"

namespace aech::graphics
{
	void renderer_t::generate_environment_cubemap()
	{
		auto equirectangular_map = resource_manager::load_hdr_texture("equirectangular_map", "textures_pbr/hdr/newport_loft.hdr");

		auto       capture_projection = math::perspective(90, 1, 0.1f, 100.0f);
		std::array capture_views
		{
			math::look_at({0, 0, 0}, {1, 0, 0}, {0, 1, 0}),
			math::look_at({0, 0, 0}, {-1, 0, 0}, {0, 1, 0}),
			math::look_at({0, 0, 0}, {0, 1, 0}, {0, 0, -1}),
			math::look_at({0, 0, 0}, {0, -1, 0}, {0, 0, 1}),
			math::look_at({0, 0, 0}, {0, 0, 1}, {0, 1, 0}),
			math::look_at({0, 0, 0}, {0, 0, -1}, {0, 1, 0})

		};

		auto& shader = resource_manager::shaders["hdr_to_cubemap"];
		shader.use();
		shader.set_uniform("equirectangular_map", 0);
		shader.set_uniform("projection", capture_projection);
		equirectangular_map->bind(0);

		auto& fbo = framebuffer_cubes["hdr_capture"];
		fbo.bind();
		resource_manager::texture_cubes["environment"].bind(0);
		glViewport(0, 0, fbo.width, fbo.height);

		auto& ndc_cube = *mesh_library::default_meshes["cube"];
		glBindVertexArray(ndc_cube.m_vao);
		for (size_t i = 0; i < 6; i++)
		{
			shader.set_uniform("view", capture_views[i]);
			fbo.attach(i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawElements(GL_TRIANGLES, ndc_cube.m_indices.size(), GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);
		fbo.unbind();
	}

	renderer_t::renderer_t()
	{
		material_library::generate_default_materials();
		mesh_library::generate_default_meshes();

		generate_default_framebuffers();

		opaque_renderer = engine.register_system<opaque_renderer_t>();
		{
			signature_t signature{};
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			signature.set(engine.get_component_type<opaque_t>());
			engine.set_system_signature<opaque_renderer_t>(signature);
		}

		opaque_shadow_renderer = engine.register_system<opaque_shadow_renderer_t>();
		{
			signature_t signature{};
			signature.set(engine.get_component_type<shadow_caster_t>());
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<opaque_t>());
			engine.set_system_signature<opaque_shadow_renderer_t>(signature);
		}

		transparent_shadow_renderer = engine.register_system<transparent_shadow_renderer_t>();
		{
			signature_t signature{};
			signature.set(engine.get_component_type<shadow_caster_t>());
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<transparent_t>());
			engine.set_system_signature<transparent_shadow_renderer_t>(signature);
		}

		directional_light_renderer = engine.register_system<directional_light_renderer_t>();
		{
			signature_t signature{};
			signature.set(engine.get_component_type<directional_light_t>());
			signature.set(engine.get_component_type<transform_t>());
			engine.set_system_signature<directional_light_renderer_t>(signature);
		}

		point_light_renderer = engine.register_system<point_light_renderer_t>();
		{
			signature_t signature{};
			signature.set(engine.get_component_type<point_light_t>());
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			engine.set_system_signature<point_light_renderer_t>(signature);
		}

		transparent_renderer = engine.register_system<transparent_renderer_t>();
		{
			signature_t signature{};
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			signature.set(engine.get_component_type<transparent_t>());
			engine.set_system_signature<transparent_renderer_t>(signature);
		}

		m_camera = engine.create_entity();
		engine.add_component(
			m_camera,
			transform_t{
				{0.0f, 0.0f, 0.0f}
			}
		);

		engine.add_component(
			m_camera,
			camera_t{
			camera_t::make_perspective_projection(90.0f, 0.1f, 1000.0f, 1280, 720)
			}
		);

		auto dirlight = engine.create_entity();
		engine.add_component(dirlight, directional_light_t{ {1, 1,1}, 5 });
		engine.add_component(dirlight, transform_t{ {0, 1750, 0}, {-80, -10, -10}, });

		opaque_shadow_renderer->dirlight = dirlight;
		transparent_shadow_renderer->dirlight = dirlight;

		opaque_renderer->m_camera = m_camera;
		point_light_renderer->m_camera = m_camera;
		transparent_renderer->m_camera = m_camera;
		transparent_renderer->dirlight = dirlight;

		directional_light_renderer->mesh_filter.material->set_texture("texture_position", &opaque_renderer->render_target->m_colour_attachments[0], 0);
		directional_light_renderer->mesh_filter.material->set_texture("texture_normal", &opaque_renderer->render_target->m_colour_attachments[1], 1);
		directional_light_renderer->mesh_filter.material->set_texture("texture_albedo", &opaque_renderer->render_target->m_colour_attachments[2], 2);
		directional_light_renderer->mesh_filter.material->set_texture("texture_metallic_roughness_ao", &opaque_renderer->render_target->m_colour_attachments[3], 3);
		directional_light_renderer->mesh_filter.material->set_texture("light_shadow_map", &opaque_shadow_renderer->shadow_map->m_colour_attachments[0], 4);

		transparent_renderer->mesh_filter.material->set_texture("light_shadow_map", &opaque_shadow_renderer->shadow_map->m_colour_attachments[0], 4);

		//generate_environment_cubemap();
	}

	void renderer_t::render_environment_cube()
	{
		auto equirectangular_map = resource_manager::load_hdr_texture("equirectangular_map", "textures_pbr/hdr/newport_loft.hdr");

		auto       capture_projection = math::perspective(90, 1, 0.1f, 100.0f);
		std::array capture_views
		{
			math::look_at({0, 0, 0}, {1, 0, 0}, {0, -1, 0}),
			math::look_at({0, 0, 0}, {-1, 0, 0}, {0,-1, 0}),
			math::look_at({0, 0, 0}, {0, 1, 0}, {0, 0, 1}),
			math::look_at({0, 0, 0}, {0, -1, 0}, {0, 0, -1}),
			math::look_at({0, 0, 0}, {0, 0, 1}, {0, -1, 0}),
			math::look_at({0, 0, 0}, {0, 0, -1}, {0, -1, 0})

		};
		glDepthFunc(GL_LEQUAL);


		auto& shader = resource_manager::shaders["hdr_to_cubemap"];
		shader.use();
		shader.set_uniform("equirectangular_map", 0);
		shader.set_uniform("projection", capture_projection);
		equirectangular_map->bind(0);

		auto& fbo = framebuffer_cubes["hdr_capture"];
		fbo.bind();
		resource_manager::texture_cubes["environment"].bind(0);
		glViewport(0, 0, fbo.width, fbo.height);

		auto& ndc_cube = *mesh_library::default_meshes["cube"];
		glBindVertexArray(ndc_cube.m_vao);
		for (size_t i = 0; i < 6; i++)
		{
			shader.set_uniform("view", capture_views[i]);
			fbo.attach(i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, ndc_cube.m_positions.size());
		}

		resource_manager::texture_cubes["environment"].generate_mips();

		glBindVertexArray(0);
		fbo.unbind();






		auto& cube    = *mesh_library::default_meshes["cube"];
		auto& shadder = resource_manager::shaders["background"];

		shadder.use();
		shadder.set_uniform("projection", engine.get_component<camera_t>(m_camera).projection);
		shadder.set_uniform("view", math::get_view_matrix(engine.get_component<transform_t>(m_camera)));

		resource_manager::texture_cubes["environment"].bind(0);
		shadder.set_uniform("environment_map", 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screen_width, screen_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(cube.m_vao);
		glDrawArrays(GL_TRIANGLES, 0, ndc_cube.m_positions.size());
	}

	void renderer_t::update()
	{
		// 1. render to g buffer
		opaque_renderer->update();

		// 2. render shadows
		// TODO: fix shadows
		opaque_shadow_renderer->update();
		transparent_shadow_renderer->update();
		// 4. render lights

		directional_light_renderer->update();

		//glCullFace(GL_FRONT);
		point_light_renderer->update();
		//glCullFace(GL_BACK);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, transparent_renderer->render_target->id);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, opaque_renderer->render_target->id);
		glBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);


		transparent_renderer->update();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, transparent_renderer->render_target->id);
		glBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		// 5. forward rendering
	}
}
