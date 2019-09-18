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

	renderer_t::renderer_t()
	{
		material_library::generate_default_materials();
		mesh_library::generate_default_meshes();
		generate_default_framebuffers();

		ndc_cube = mesh_library::default_meshes["cube"].get();

		hdr_to_cubemap_shader = &resource_manager::shaders["hdr_to_cubemap"];
		irradiance_shader = &resource_manager::shaders["irradiance"];

		hdr_capture_fbo = &framebuffer_cubes["hdr_capture"];
		irradiance_fbo = &framebuffer_cubes["precomputed_irradiance"];

		specular_prefilter_shader = &resource_manager::shaders["prefilter"];

		light_probe_renderer = engine.register_system<light_probe_renderer_t>();
		{
			// for now, testing
			signature_t signature{};
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			engine.set_system_signature<light_probe_renderer_t>(signature);
		}

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
				math::perspective(90.0f, 1280.0F/720, 0.1f, 4000.0f)
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

		directional_light_renderer->mesh_filter.material()->set_texture("texture_position", &opaque_renderer->render_target->colour_attachments()[0], 0);
		directional_light_renderer->mesh_filter.material()->set_texture("texture_normal", &opaque_renderer->render_target->colour_attachments()[1], 1);
		directional_light_renderer->mesh_filter.material()->set_texture("texture_albedo", &opaque_renderer->render_target->colour_attachments()[2], 2);
		directional_light_renderer->mesh_filter.material()->set_texture("texture_metallic_roughness_ao", &opaque_renderer->render_target->colour_attachments()[3], 3);
		directional_light_renderer->mesh_filter.material()->set_texture("light_shadow_map", &opaque_shadow_renderer->shadow_map->colour_attachments()[0], 4);
		
		light_probe_renderer->ambient_material->set_texture("texture_position", &opaque_renderer->render_target->colour_attachments()[0], 0);
		light_probe_renderer->ambient_material->set_texture("texture_normal", &opaque_renderer->render_target->colour_attachments()[1], 1);
		light_probe_renderer->ambient_material->set_texture("texture_albedo", &opaque_renderer->render_target->colour_attachments()[2], 2);
		light_probe_renderer->ambient_material->set_texture("texture_metallic_roughness_ao", &opaque_renderer->render_target->colour_attachments()[3], 3);
		light_probe_renderer->ambient_material->set_texture("light_shadow_map", &opaque_shadow_renderer->shadow_map->colour_attachments()[0], 4);

		transparent_renderer->mesh_filter.material()->set_texture("light_shadow_map", &opaque_shadow_renderer->shadow_map->colour_attachments()[0], 4);

		// bottom centre
		light_probe_renderer->light_probes.push_back(light_probe_t{ {0, 150, -50}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {300, 150, -50}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {600, 150, -50}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {850, 150, -50}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {1140, 150, -50}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-300, 150, -50}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-620, 150, -50}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-950, 150, -50}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-1210, 150, -50}, 400 });

		// bottom left 
		light_probe_renderer->light_probes.push_back(light_probe_t{ {0, 150, 400}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {300, 150, 400}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {600, 150, 400}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {850, 150, 400}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {1140, 150, 400}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-300, 150, 400}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-620, 150, 400}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-950, 150, 400}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-1210, 150, 400}, 400 });

		//bottom right
		light_probe_renderer->light_probes.push_back(light_probe_t{ {0, 150, -450}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {300, 150, -450}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {600, 150,-450}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {850, 150,-450}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {1140, 150, -450}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-300, 150, -450}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-620, 150, -450}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-950, 150, -450}, 400 });
		light_probe_renderer->light_probes.push_back(light_probe_t{ {-1210, 150, -450}, 400 });


		light_probe_renderer->camera_transform = &engine.get_component<transform_t>(m_camera);
		light_probe_renderer->camera = &engine.get_component<camera_t>(m_camera);

		auto skyent = engine.create_entity();
		engine.add_component(skyent, mesh_filter_t{ mesh_library::default_meshes["cube"].get(), &material_library::default_materials["skybox"] });
		engine.add_component(skyent, transform_t{});
		engine.add_component(skyent, opaque_t{});
		engine.add_component(skyent, scene_node_t{&engine.get_component<transform_t>(skyent)});

		//TODO: refactor this
		auto skybox = resource_manager::load_hdr_texture("skybox", "textures_pbr/hdr/skybox.hdr");
		const static auto capture_projection = math::perspective(90, 1, 0.1f, 10.0f);
		const std::array capture_views
		{
			math::look_at({}, math::vec3_t{1, 0, 0}, {0, -1, 0}),
			math::look_at({}, math::vec3_t{-1, 0, 0}, {0, -1, 0}),
			math::look_at({},  math::vec3_t{0, 1, 0}, {0, 0, 1}),
			math::look_at({}, math::vec3_t{0, -1, 0}, {0, 0, -1}),

			math::look_at({},  math::vec3_t{0, 0, 1}, {0, -1, 0}),
			math::look_at({}, math::vec3_t{0, 0, -1}, {0, -1, 0})
		};

		auto sky = &resource_manager::texture_cubes["skybox"];
		sky->width = 1024;
		sky->height = 1024;
		sky->sized_internal_format = texture_types::sized_internal_format::rgba32f;
		sky->format = texture_types::format::rgba;
		sky->type = texture_types::type::floating_point;
		sky->init();
		framebuffer_cube_t fbo{ &resource_manager::texture_cubes["skybox"], 1024, 1024 };

		fbo.bind();
		glViewport(0, 0, fbo.width, fbo.height);

		hdr_to_cubemap_shader->use();
		hdr_to_cubemap_shader->set_uniform("equirectangular_map", 0);
		skybox->bind(0);

		for (size_t i = 0; i < 6; i++)
		{
			fbo.attach(i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			hdr_to_cubemap_shader->set_uniform("projection", capture_projection);
			hdr_to_cubemap_shader->set_uniform("view", capture_views[i]);
			ndc_cube->draw();
		}
		fbo.unbind();

		sky->generate_mips();

		auto mat = &material_library::default_materials["skybox"];
		mat->set_texture_cube("skybox", &resource_manager::texture_cubes["skybox"], 0);
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

		light_probe_renderer->render_ambient_pass();

		directional_light_renderer->update();

		glCullFace(GL_FRONT);
		point_light_renderer->update();
		glCullFace(GL_BACK);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, transparent_renderer->render_target->id());
		glBindFramebuffer(GL_READ_FRAMEBUFFER, opaque_renderer->render_target->id());
		glBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		transparent_renderer->update();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, transparent_renderer->render_target->id());
		glBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		// 5. forward rendering
	}
}
