#include "renderer.hpp"
#include "camera.hpp"
#include <array>
#include "directional_light.hpp"
#include "material_library.hpp"
#include "mesh_library.hpp"
#include "point_light.hpp"
#include "shading_tags.hpp"
#include "shadow_caster.hpp"
#include "transforms.hpp"
#include "transparent_shadow_renderer.hpp"
#include "main.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <iostream>
#include <random>
#include "vector_math.hpp"


namespace aech::graphics
{
	void renderer_t::init()
	{
		material_library::generate_default_materials();
		mesh_library::generate_default_meshes();
		generate_default_framebuffers();

		post_process_fbo = &framebuffers["post_process"];

		tonemap_shader = &resource_manager::shaders["tonemap"];

		m_gbuffer          = &framebuffers["g_buffer"];
		m_ssao_blurred_fbo = &framebuffers["ssao_blurred"];
		m_ssao_fbo         = &framebuffers["ssao"];

		m_ssao_shader      = &resource_manager::shaders["ssao"];
		m_ssao_blur_shader = &resource_manager::shaders["ssao_blur"];

		ndc_cube    = mesh_library::default_meshes["cube"].get();
		screen_quad = mesh_library::default_meshes["quad"].get();

		hdr_to_cubemap_shader = &resource_manager::shaders["hdr_to_cubemap"];
		irradiance_shader     = &resource_manager::shaders["irradiance"];

		hdr_capture_fbo = &framebuffer_cubes["hdr_capture"];
		irradiance_fbo  = &framebuffer_cubes["precomputed_irradiance"];

		specular_prefilter_shader = &resource_manager::shaders["prefilter"];

		post_process_shader = &resource_manager::shaders["post_process"];

		auto dirlight = engine.create_entity();
		engine.add_component(dirlight, transform_t{{0, 17.50, 0}, {-75, 10, -10}});
		engine.add_component(dirlight, directional_light_t{{1, 1, 1}, 5, &engine.get_component<transform_t>(dirlight)});

		auto dir    = &engine.get_component<directional_light_t>(dirlight);

		auto m_camera_entity = engine.create_entity();
		engine.add_component(m_camera_entity, transform_t{{0.0F, 0.0F, -1.0F}});
		auto ptr_camera_transform = &engine.get_component<transform_t>(m_camera_entity);
		
		engine.add_component(m_camera_entity, scene_node_t {
			ptr_camera_transform,
		});
		
		engine.add_component(m_camera_entity,
		                     camera_t{
			                     math::perspective(90.0F, 1920.0F / 800.0F, 0.01F, 1000.0F),
			                     ptr_camera_transform,
		                     });

		m_camera = &engine.get_component<camera_t>(m_camera_entity);
		
		gi_renderer = engine.register_system<gi_renderer_t>(render_cache(), m_camera);
		{
			// for now, testing
			signature_t signature{};
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			engine.set_system_signature<gi_renderer_t>(signature);
		}

		opaque_renderer = engine.register_system<opaque_renderer_t>(render_cache(), m_camera);
		{
			signature_t signature{};
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			signature.set(engine.get_component_type<opaque_t>());
			engine.set_system_signature<opaque_renderer_t>(signature);
		}

		opaque_shadow_renderer = engine.register_system<opaque_shadow_renderer_t>(render_cache(), dir);
		{
			signature_t signature{};
			signature.set(engine.get_component_type<potential_occluder_t>());
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<opaque_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			engine.set_system_signature<opaque_shadow_renderer_t>(signature);
		}

		transparent_shadow_renderer = engine.register_system<transparent_shadow_renderer_t>(render_cache(), dir);
		{
			signature_t signature{};
			signature.set(engine.get_component_type<potential_occluder_t>());
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<transparent_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			engine.set_system_signature<transparent_shadow_renderer_t>(signature);
		}

		m_directional_light_renderer = std::make_unique<directional_light_renderer_t>(render_cache(), dir);

		point_light_renderer = engine.register_system<point_light_renderer_t>(render_cache(), m_camera);
		{
			signature_t signature{};
			signature.set(engine.get_component_type<point_light_t>());
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			// TODO: signature.set(engine.get_component_type<scene_node_t>());
			engine.set_system_signature<point_light_renderer_t>(signature);
		}

		transparent_renderer = engine.register_system<transparent_renderer_t>(render_cache(), m_camera, dir);
		{
			signature_t signature{};
			signature.set(engine.get_component_type<transform_t>());
			signature.set(engine.get_component_type<mesh_filter_t>());
			signature.set(engine.get_component_type<scene_node_t>());
			signature.set(engine.get_component_type<transparent_t>());
			engine.set_system_signature<transparent_renderer_t>(signature);
		}

		m_directional_light_renderer->mesh_filter().material()->set_texture("texture_position",
		                                                                    &opaque_renderer->
		                                                                     render_target()->colour_attachments()[0],
		                                                                    0);
		m_directional_light_renderer->mesh_filter().material()->set_texture("texture_normal",
		                                                                    &opaque_renderer->
		                                                                     render_target()->colour_attachments()[1],
		                                                                    1);
		m_directional_light_renderer->mesh_filter().material()->set_texture("texture_albedo",
		                                                                    &opaque_renderer->
		                                                                     render_target()->colour_attachments()[2],
		                                                                    2);
		m_directional_light_renderer->mesh_filter().material()->set_texture("texture_metallic_roughness_ao",
		                                                                    &opaque_renderer->
		                                                                     render_target()->colour_attachments()[3],
		                                                                    3);
		m_directional_light_renderer->mesh_filter().material()->set_texture("light_shadow_map",
		                                                                    opaque_shadow_renderer->
		                                                                    render_target()->depth_and_stencil(),
		                                                                    4);

		gi_renderer->specular_material()->set_texture("texture_position",
		                                                      &opaque_renderer->render_target()->colour_attachments()[0
		                                                      ],
		                                                      0);
		gi_renderer->specular_material()->set_texture("texture_normal",
		                                                      &opaque_renderer->render_target()->colour_attachments()[1
		                                                      ],
		                                                      1);
		gi_renderer->specular_material()->set_texture("texture_albedo",
		                                                      &opaque_renderer->render_target()->colour_attachments()[2
		                                                      ],
		                                                      2);
		gi_renderer->specular_material()->set_texture("texture_metallic_roughness_ao",
		                                                      &opaque_renderer->render_target()->colour_attachments()[3
		                                                      ],
		                                                      3);

		gi_renderer->diffuse_material()->set_texture("texture_position",
			&opaque_renderer->render_target()->colour_attachments()[0
			],
			0);
		gi_renderer->diffuse_material()->set_texture("texture_normal",
			&opaque_renderer->render_target()->colour_attachments()[1
			],
			1);
		gi_renderer->diffuse_material()->set_texture("texture_albedo",
			&opaque_renderer->render_target()->colour_attachments()[2
			],
			2);
		gi_renderer->diffuse_material()->set_texture("texture_metallic_roughness_ao",
			&opaque_renderer->render_target()->colour_attachments()[3
			],
			3);
		m_render_cache.set_shader(m_ssao_shader);
		m_ssao_shader->set_uniform("texture_position", opaque_renderer->render_target()->colour_attachments()[0]);
		m_ssao_shader->set_uniform("texture_normal", opaque_renderer->render_target()->colour_attachments()[1]);

		auto probe1 = engine.create_entity();
		engine.add_component(probe1, transform_t{ {0, 1.00, -0.5 }, {}, {20.00, 4.00, 4.20} });;
		engine.add_component(probe1, scene_node_t{&engine.get_component<transform_t>(probe1)});
		engine.add_component(probe1, light_probe_t{ {0, 1.50, -.40}, 2.50F, 5.00F, &engine.get_component<scene_node_t>(probe1) });
		engine.get_component<scene_node_t>(probe1).set_aabb(mesh_library::default_meshes["cube"].get()->calculate_aabb());
		gi_renderer->add_probe(engine.get_component<light_probe_t>(probe1));

		auto probe2 = engine.create_entity();
		engine.add_component(probe2, transform_t{ {0, 1.00, -0.5 }, {}, {20.00, 4.00, 4.20} });;
		engine.add_component(probe2, scene_node_t{ &engine.get_component<transform_t>(probe2) });
		engine.add_component(probe2, light_probe_t{ {5.0, 1.50, -.40}, 2.50F, 5.00F, &engine.get_component<scene_node_t>(probe2) });
		engine.get_component<scene_node_t>(probe2).set_aabb(mesh_library::default_meshes["cube"].get()->calculate_aabb());
		gi_renderer->add_probe(engine.get_component<light_probe_t>(probe2));

		auto probe3 = engine.create_entity();
		engine.add_component(probe3, transform_t{ {0, 1.00, -0.5 }, {}, {20.00, 4.00, 4.20} });;
		engine.add_component(probe3, scene_node_t{ &engine.get_component<transform_t>(probe3) });
		engine.add_component(probe3, light_probe_t{ {-5.0, 1.50, -.40}, 2.50F, 5.00F, &engine.get_component<scene_node_t>(probe3) });
		engine.get_component<scene_node_t>(probe3).set_aabb(mesh_library::default_meshes["cube"].get()->calculate_aabb());
		gi_renderer->add_probe(engine.get_component<light_probe_t>(probe3));
		auto probe11 = engine.create_entity();
		engine.add_component(probe11, transform_t{ {-.2, 1.00 , -.5}, {}, {19.40, 4.00, 4.20} });;
		engine.add_component(probe11, scene_node_t{ &engine.get_component<transform_t>(probe11) });
		engine.add_component(probe11, reflection_probe_t{ {0, 1.50, -.5}, &engine.get_component<scene_node_t>(probe11) });
		engine.get_component<scene_node_t>(probe11).set_aabb(mesh_library::default_meshes["cube"].get()->calculate_aabb());
		gi_renderer->add_probe(engine.get_component<reflection_probe_t>(probe11));
			
		//// bottom centre
		//light_probe_renderer->add_probe(light_probe_t{{0, 15.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{30.0, 15.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{60.0, 15.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{85.0, 15.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{114.0, 15.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-30.0, 15.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-62.0, 15.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-95.0, 15.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-121.0, 15.0, -5.0}, 40.0});

		//// bottom left 
		//light_probe_renderer->add_probe(light_probe_t{{0, 15.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{30.0, 15.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{60.0, 15.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{85.0, 15.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{114.0, 15.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-30.0, 15.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-62.0, 15.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-95.0, 15.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-121.0, 15.0, 40.0}, 40.0});

		////bottom right
		//light_probe_renderer->add_probe(light_probe_t{{0, 15.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{30.0, 15.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{60.0, 15.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{85.0, 15.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{114.0, 15.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-30.0, 15.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-62.0, 15.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-95.0, 15.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-121.0, 15.0, -45.0}, 40.0});

		//// middle centre
		//light_probe_renderer->add_probe(light_probe_t{{0, 60.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{30.0, 60.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{60.0, 60.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{85.0, 60.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{114.0, 60.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-30.0, 60.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-62.0, 60.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-95.0, 60.0, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-121.0, 60.0, -5.0}, 40.0});


		//// middle left 
		//light_probe_renderer->add_probe(light_probe_t{{0, 60.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{30.0, 60.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{60.0, 60.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{85.0, 60.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{114.0, 60.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-30.0, 60.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-62.0, 60.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-95.0, 60.0, 40.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-121.0, 60.0, 40.0}, 40.0});

		//// middle right
		//light_probe_renderer->add_probe(light_probe_t{{0, 60.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{30.0, 60.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{60.0, 60.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{85.0, 60.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{114.0, 60.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-30.0, 60.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-62.0, 60.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-95.0, 60.0, -45.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-121.0, 60.0, -45.0}, 40.0});


		//// top centre
		//light_probe_renderer->add_probe(light_probe_t{{0, 105, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{40.0, 105, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{70.0, 105, -5.0}, 30.0});
		//light_probe_renderer->add_probe(light_probe_t{{-40.0, 105, -5.0}, 40.0});
		//light_probe_renderer->add_probe(light_probe_t{{-80.0, 105, -5.0}, 40.0});

		//auto player_object = engine.create_entity();
		//engine.add_component(player_object, transform_t{});
		//auto ptr_player_transform = &engine.get_component<transform_t>(player_object);
		//
		//engine.add_component(player_object, scene_node_t {
		//	ptr_player_transform
		//});

		//auto ptr_player_scene_node = &engine.get_component<scene_node_t>(player_object);
		//ptr_player_scene_node->set_scale(10.0F);
		//ptr_player_scene_node->set_position({0.0F, 10.0F, -1.0F});
		//ptr_player_scene_node->set_aabb(mesh_library::default_meshes["cube"].get()->calculate_aabb());
		//
		//engine.add_component(player_object,
		//	mesh_filter_t
		//	{
		//		mesh_library::default_meshes["cube"].get(),
		//		&material_library::default_materials["basic"]
		//	}
		//);

		//engine.add_component(player_object,
		//	potential_occluder_t{}
		//);

		//engine.add_component(player_object, opaque_t{});

		input_manager.set_camera(m_camera);

		
		// TODO(Marko): refactor this

		const static auto capture_projection = math::perspective(90, 1, 0.1F, 10.0F);
		const std::array  capture_views
		{
			look_at({}, math::vec3_t{1, 0, 0}, {0, -1, 0}),
			look_at({}, math::vec3_t{-1, 0, 0}, {0, -1, 0}),
			look_at({}, math::vec3_t{0, 1, 0}, {0, 0, 1}),
			look_at({}, math::vec3_t{0, -1, 0}, {0, 0, -1}),

			look_at({}, math::vec3_t{0, 0, 1}, {0, -1, 0}),
			look_at({}, math::vec3_t{0, 0, -1}, {0, -1, 0})
		};

		auto skybox = resource_manager::load_hdr_texture("skybox", "textures_pbr/hdr/skybox.hdr");
		auto sky    = &resource_manager::texture_cubes["skybox"];
		*sky        = texture_cube_t{
			1024,
			1024,
			texture_types::sized_internal_format::rgba16f,
			texture_types::format::rgba,
			texture_types::type::floating_point
		};
		framebuffer_cube_t fbo{&resource_manager::texture_cubes["skybox"], 1024, 1024};

		fbo.bind();
		m_render_cache.set_viewport(0, 0, fbo.width(), fbo.height());
		m_render_cache.set_cull(false);
		m_render_cache.set_shader(hdr_to_cubemap_shader);
		hdr_to_cubemap_shader->set_uniform("equirectangular_map", 0);
		skybox->bind(0);

		for (uint32_t i = 0; i < 6; i++)
		{
			fbo.attach(i);
			m_render_cache.clear(clear::color_and_depth_buffer_bit);
			hdr_to_cubemap_shader->set_uniform("projection", capture_projection);
			hdr_to_cubemap_shader->set_uniform("view", capture_views[i]);
			ndc_cube->draw();
		}
		framebuffer_cube_t::unbind();

		sky->generate_mips();

		auto mat = &material_library::default_materials["skybox"];
		mat->set_texture_cube("skybox", &resource_manager::texture_cubes["skybox"], 0);

		// ssao

		std::uniform_real_distribution zero_to_one{0.0F, 1.0F};
		std::default_random_engine            random_engine{};

		for (uint32_t i = 0; i < ssao_kernel_size; i++)
		{
			math::vec3_t sample{zero_to_one(random_engine) * 2 - 1, zero_to_one(random_engine) * 2 - 1, zero_to_one(random_engine)};
			sample      = zero_to_one(random_engine) * normalize(sample);
			auto scale = static_cast<float>(i) / ssao_kernel_size;
			scale       = math::lerp(0.1F, 1.0F, scale * scale);
			ssao_kernel.push_back(scale * sample);
		}

		std::vector<math::vec3_t> ssao_noise{};
		for (uint32_t i = 0; i < 16; i++)
			ssao_noise.emplace_back(zero_to_one(random_engine) * 2 - 1, zero_to_one(random_engine) * 2 - 1, 0.F);

		ssao_noise_texture = std::make_unique<texture_t>(4,
		                                                 4,
		                                                 texture_types::sized_internal_format::rgb16f,
		                                                 texture_types::format::rgb,
		                                                 texture_types::type::floating_point,
		                                                 false,
		                                                 texture_types::filtering::linear,
		                                                 texture_types::filtering::linear,
		                                                 ssao_noise.data());
		m_render_cache.set_shader(m_ssao_shader);
		m_ssao_shader->set_uniform("texture_noise", ssao_noise_texture->id());
	}


	void renderer_t::bake_probes()
	{
		gi_renderer->bake_probes();
	}


	bool renderer_t::options() const
	{
		return m_options;
	}


	bool renderer_t::sort_front_to_back(entity_t a, entity_t b)
	{
		auto &scene_node_a = engine.get_component<scene_node_t>(a);
		auto &scene_node_b = engine.get_component<scene_node_t>(b);
		auto aabb_a = scene_node_a.bounding_box();
		auto aabb_b = scene_node_b.bounding_box();

		auto centroid_a = math::vec3_t{(aabb_a.min_coords + aabb_a.max_coords)/2.0F};
		auto centroid_b = math::vec3_t{(aabb_b.min_coords + aabb_b.max_coords)/2.0F};
		
		return math::distance_squared(centroid_a, renderer.m_camera->transform()->position)
			< math::distance_squared(centroid_b, renderer.m_camera->transform()->position);
	}


	bool renderer_t::sort_back_to_front(entity_t a, entity_t b)
	{
		auto &scene_node_a = engine.get_component<scene_node_t>(a);
		auto &scene_node_b = engine.get_component<scene_node_t>(b);
		auto aabb_a = scene_node_a.bounding_box();
		auto aabb_b = scene_node_b.bounding_box();

		auto centroid_a = math::vec3_t{(aabb_a.min_coords + aabb_a.max_coords)/2.0F};
		auto centroid_b = math::vec3_t{(aabb_b.min_coords + aabb_b.max_coords)/2.0F};
		
		return math::distance_squared(centroid_a, renderer.m_camera->transform()->position)
			> math::distance_squared(centroid_b, renderer.m_camera->transform()->position);
	}

	void renderer_t::set_options(bool gui)
	{
		m_options = gui;
	}

	float renderer_t::poisson_sampling_distance() const
	{
		return m_poisson_sampling_distance;
	}


	bool renderer_t::ssao() const
	{
		return m_ssao;
	}


	render_cache_t* renderer_t::render_cache()
	{
		return &m_render_cache;
	}

	const texture_t* renderer_t::ssao_texture() const
	{
		return &m_ssao_blurred_fbo->colour_attachments().front();
	}

	bool renderer_t::shadows() const
	{
		return m_shadows;
	}

	void renderer_t::post_process()
	{
		post_process_fbo->bind();
		m_render_cache.clear(clear::color_and_depth_buffer_bit);
		m_render_cache.set_depth_test(false);
		m_render_cache.set_shader(tonemap_shader);
		m_directional_light_renderer->render_target()->colour_attachments().front().bind(0);
		tonemap_shader->set_uniform("tex", 0);
		screen_quad->draw();
		m_render_cache.set_depth_test(true);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_render_cache.clear(clear::color_and_depth_buffer_bit);
		m_render_cache.set_depth_test(false);
		m_render_cache.set_shader(post_process_shader);
		post_process_fbo->colour_attachments().front().bind(0);
		post_process_shader->set_uniform("source", 0);
		post_process_shader->set_uniform("use_fxaa", fxaa);
		post_process_shader->set_uniform("resolution",
		                                 math::vec2_t{
			                                 static_cast<float>(window_manager.width()),
			                                 static_cast<float>(window_manager.height())
		                                 });

		screen_quad->draw();
		m_render_cache.set_depth_test(true);
	}

	void renderer_t::render_gui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("aech");
		// TODO: why does this give completely different results?
		ImGui::Text("average fps: %.2f fps", ImGui::GetIO().Framerate);
		ImGui::Text("average frametime: %.2f ms", 1000.0F / ImGui::GetIO().Framerate);
		ImGui::Text("press 'o' to toggle options");
		ImGui::End();

		if (m_options)
		{
			ImGui::Begin("options");

			ImGui::Checkbox("diffuse gi", &gi_renderer->m_diffuse_gi);
			ImGui::NewLine();

			ImGui::Checkbox("specular gi", &gi_renderer->m_specular_gi);
			ImGui::NewLine();
			
			ImGui::Checkbox("shadows", &m_shadows);
			ImGui::SliderFloat("shadow sampling radius", &m_poisson_sampling_distance, 0.0F, 10.0F, "%.3f");
			ImGui::NewLine();
			
			ImGui::Checkbox("fxaa", &fxaa);
			ImGui::NewLine();
			
			ImGui::Checkbox("ssao", &m_ssao);
			ImGui::SliderFloat("ssao hemisphere radius", &m_ssao_hemisphere_sampling_radius, 0.0F, 100.0F, "%.3f");

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void renderer_t::render_ssao()
	{
		m_ssao_fbo->bind();
		m_render_cache.set_shader(m_ssao_shader);
		m_render_cache.set_depth_test(false);
		m_render_cache.set_cull(false);
		m_render_cache.set_blend(false);
		m_render_cache.set_viewport(0, 0, m_ssao_fbo->width(), m_ssao_fbo->height());
		m_render_cache.clear(clear::color_and_depth_buffer_bit);

		m_gbuffer->colour_attachments()[0].bind(0);
		m_gbuffer->colour_attachments()[1].bind(1);
		m_ssao_shader->set_uniform("texture_position", 0);
		m_ssao_shader->set_uniform("texture_normal", 1);
		ssao_noise_texture->bind(2);
		m_ssao_shader->set_uniform("texture_noise", 2);
		m_ssao_shader->set_uniform("projection", m_camera->projection());
		m_ssao_shader->set_uniform("view", m_camera->view_matrix());
		m_ssao_shader->set_uniform("resolution",
		                           math::vec2_t{
			                           static_cast<float>(window_manager.width()),
			                           static_cast<float>(window_manager.height())
		                           });
		m_ssao_shader->set_uniform("radius", m_ssao_hemisphere_sampling_radius);
		for (size_t i = 0; i < ssao_kernel.size(); i++)
			m_ssao_shader->set_uniform("samples[" + std::to_string(i) + "]", ssao_kernel[i]);
		screen_quad->draw();

		m_ssao_blurred_fbo->bind();
		m_render_cache.clear(clear::color_and_depth_buffer_bit);
		m_render_cache.set_shader(m_ssao_blur_shader);
		m_ssao_fbo->colour_attachments()[0].bind(0);
		m_ssao_blur_shader->set_uniform("ssao_input", 0);
		screen_quad->draw();
	}

	void renderer_t::update()
	{
		// 1. render to g buffer
		opaque_renderer->update();
		transparent_renderer->update();

		// 2. shadows
		if (m_shadows)
		{
			// create shadow map
			opaque_shadow_renderer->update();
			transparent_shadow_renderer->update();
		}

		if (m_ssao)
		{
			render_ssao();
		}

		// 3. render ambient lighting
		gi_renderer->render_ambient_pass();

		// 4. render direct lighting
		m_directional_light_renderer->update();

		m_render_cache.set_cull_face(cull_face::front);
		point_light_renderer->update();
		m_render_cache.set_cull_face(cull_face::back);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_directional_light_renderer->render_target()->id());
		glBindFramebuffer(GL_READ_FRAMEBUFFER, opaque_renderer->render_target()->id());
		glBlitFramebuffer(0,
		                  0,
		                  window_manager.width(),
		                  window_manager.height(),
		                  0,
		                  0,
		                  window_manager.width(),
		                  window_manager.height(),
		                  GL_DEPTH_BUFFER_BIT,
		                  GL_NEAREST);

		m_directional_light_renderer->render_target()->bind();
		// 6. render skybox over the whole scene
		opaque_renderer->draw_skybox();

		// 7. tonemapping and fxaa
		post_process();

		// 8. render gui
		render_gui();
	}
} // namespace aech::graphics
