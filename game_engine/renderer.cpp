#include "renderer.hpp"
#include "mesh_library.hpp"
#include "material_library.hpp"
#include "camera.hpp"
#include "directional_light.hpp"
#include "point_light.hpp"
#include "shadow_caster.hpp"
#include <iostream>

namespace aech::graphics
{
	renderer_t::renderer_t()
	{
		material_library::generate_default_materials();
		mesh_library::generate_default_meshes();
		generate_default_framebuffers();



		g_buffer_renderer = engine.register_system<g_buffer_renderer_t>();
		{
			signature_t signature{};
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			engine.set_system_signature<g_buffer_renderer_t>(signature);
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

		shadow_renderer = engine.register_system<shadow_renderer_t>();
		{
			signature_t signature{};
			signature.set(engine.get_component_type<shadow_caster_t>());
			signature.set(engine.get_component_type<transform_t>());
			engine.set_system_signature<shadow_renderer_t>(signature);
		}

		m_camera = engine.create_entity();
		engine.add_component(
			m_camera,
			transform_t{
				{0.0f, 0.0f, 400.0f}
			}
		);

		engine.add_component(
			m_camera,
			camera_t{
			camera_t::make_perspective_projection(60.0f, 0.1f, 1000.0f, 1280, 720)
			}
		);

		auto dirlight = engine.create_entity();
		engine.add_component(dirlight, directional_light_t{ {1, 1,1}, 10 });
		engine.add_component(dirlight, transform_t{ {0, 1750, 0}, {-80, -10, -10}, });

		shadow_renderer->dirlight = dirlight;
		g_buffer_renderer->m_camera = m_camera;
		point_light_renderer->m_camera = m_camera;


	}

	void renderer_t::update()
	{
		// 1. render to g buffer
		g_buffer_renderer->update();

		// 2. render shadows
		shadow_renderer->update();
		// 4. render lights
		g_buffer_renderer->g_buffer->m_colour_attachments[0].bind(0);
		g_buffer_renderer->g_buffer->m_colour_attachments[1].bind(1);
		g_buffer_renderer->g_buffer->m_colour_attachments[2].bind(2);
		g_buffer_renderer->g_buffer->m_colour_attachments[3].bind(3);
		shadow_renderer->shadow_map->m_colour_attachments[0].bind(4);

		// render deferred ambient;

		directional_light_renderer->update();

		//glCullFace(GL_FRONT);
		//point_light_renderer->update();
		//glCullFace(GL_BACK);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, directional_light_renderer->render_target->id);
		glBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		// 5. forward rendering
	}
}
