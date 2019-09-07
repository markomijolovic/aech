#include "renderer.hpp"
#include "mesh_library.hpp"
#include "material_library.hpp"
#include "camera.hpp"
#include "directional_light.hpp"
#include "point_light.hpp"
#include "shadow_caster.hpp"
#include "shading_tags.hpp"
#include "transparent_shadow_renderer.hpp"

namespace aech::graphics
{
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
		//point_light_renderer->update();
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
