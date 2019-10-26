#include "material_library.hpp"
#include "resource_manager.hpp"
#include <iostream>

void aech::graphics::material_library::generate_default_materials()
{
	auto default_shader = &resource_manager::load_shader("default",
	                                                     "shaders/g_buffer_vertex.glsl",
	                                                     "shaders/g_buffer_fragment.glsl");
	auto& default_material       = default_materials["default"];
	default_materials["default"] = {default_shader};
	default_materials["default"].set_texture("texture_albedo", resource_manager::load_texture("checkerboard.png", "textures_pbr/default/checkerboard.png"), 0);
	default_materials["default"].set_texture("texture_normal", resource_manager::load_texture("norm.png", "textures_pbr/default/norm.png", false), 1);
	default_materials["default"].set_texture("texture_metallic", resource_manager::load_texture("black.png", "textures_pbr/default/black.png"), 2);
	default_materials["default"].set_texture("texture_roughness", resource_manager::load_texture("checkerboard.png", "textures_pbr/default/checkerboard.png"), 3);


	auto basic_shader = &resource_manager::load_shader("basic", 
		"shaders/basic_vertex.glsl",
		"shaders/basic_fragment.glsl"
	);
	
	auto &basic_material = default_materials["basic"];
	basic_material = {basic_shader};
	basic_material.set_uniform("colour", math::vec4_t{236.0F/255.0F, 64.0F/255.0F, 122.0F/255.0F, 1.0F});

	auto directional = &resource_manager::load_shader("directional",
	                                                  "shaders/directional_vertex.glsl",
	                                                  "shaders/directional_fragment.glsl");
	default_materials["directional"] = {directional};

	auto point = &resource_manager::load_shader("point",
	                                            "shaders/point_vertex.glsl",
	                                            "shaders/point_fragment.glsl");
	default_materials["point"] = {point};

	auto opaque_shadow_shader = &resource_manager::load_shader("opaque_shadow",
	                                                           "shaders/opaque_shadow_vertex.glsl",
	                                                           "shaders/opaque_shadow_fragment.glsl");
	default_materials["opaque_shadow"] = {opaque_shadow_shader};

	auto transparent_shadow_shader = &resource_manager::load_shader("transparent_shadow",
	                                                                "shaders/transparent_shadow_vertex.glsl",
	                                                                "shaders/transparent_shadow_fragment.glsl");
	default_materials["transparent_shadow"] = {transparent_shadow_shader};

	auto transparent_shader = &resource_manager::load_shader("transparent",
	                                                         "shaders/g_buffer_vertex.glsl",
	                                                         "shaders/g_buffer_transparent_fragment.glsl");
	default_materials["transparent"] = {transparent_shader, material_t::material_type::transparent};

	auto hdr_to_cubemap_shader = &resource_manager::load_shader("hdr_to_cubemap",
	                                                            "shaders/pbr/cube_sample_vertex.glsl",
	                                                            "shaders/pbr/equirectangular_to_cube_fragment.glsl",
	                                                            "");
	default_materials["hdr_to_cubemap"] = {hdr_to_cubemap_shader};


	auto background_shader = &resource_manager::load_shader("background",
	                                                        "shaders/background_vertex.glsl",
	                                                        "shaders/background_fragment.glsl");
	default_materials["background"] = {background_shader};


	auto irradiance_shader = &resource_manager::load_shader("precompute_irradiance",
	                                                        "shaders/pbr/cube_sample_vertex.glsl",
	                                                        "shaders/pbr/precompute_irradiance_fragment.glsl");
	default_materials["irradiance"] = {irradiance_shader};


	auto prefilter_shader = &resource_manager::load_shader("prefilter",
	                                                       "shaders/pbr/cube_sample_vertex.glsl",
	                                                       "shaders/pbr/prefilter_fragment.glsl");
	default_materials["prefilter"] = {prefilter_shader};


	auto brdf_integral_shader = &resource_manager::load_shader("brdf",
	                                                           "shaders/pbr/screen_quad_vertex.glsl",
	                                                           "shaders/pbr/brdf_fragment.glsl");
	default_materials["brdf"] = {brdf_integral_shader};

	auto probe_capture_shader = &resource_manager::load_shader("capture",
	                                                           "shaders/probe_capture_vertex.glsl",
	                                                           "shaders/probe_capture_fragment.glsl");
	default_materials["capture"] = {probe_capture_shader};

	auto probe_capture_transparent_shader = &resource_manager::load_shader("capture_transparent", "shaders/probe_capture_vertex.glsl", "shaders/probe_capture_transparent_fragment.glsl");
	default_materials["capture_transparent"] = {probe_capture_transparent_shader};
	

	auto probe_capture_skybox_shader = &resource_manager::load_shader("capture_skybox",
	                                                                  "shaders/probe_capture_skybox_vertex.glsl",
	                                                                  "shaders/probe_capture_skybox_fragment.glsl");
	default_materials["capture_skybox"] = {probe_capture_skybox_shader};


	auto ambient_shader = &resource_manager::load_shader("ambient",
	                                                     "shaders/ambient_irradiance_vertex.glsl",
	                                                     "shaders/ambient_irradiance_fragment.glsl");
	default_materials["ambient"] = {ambient_shader};


	auto skybox_shader = &resource_manager::load_shader("skybox",
	                                                    "shaders/skybox_vertex.glsl",
	                                                    "shaders/skybox_fragment.glsl");
	default_materials["skybox"] = {skybox_shader};

	auto post_processing_shader = &resource_manager::load_shader("post_process",
	                                                             "shaders/pbr/screen_quad_vertex.glsl",
	                                                             "shaders/post_processing_fragment.glsl");
	default_materials["post_process"] = {post_processing_shader};

	auto tonemap_shader = &resource_manager::load_shader("tonemap",
	                                                     "shaders/pbr/screen_quad_vertex.glsl",
	                                                     "shaders/tonemap_fragment.glsl");
	default_materials["tonemap"] = {tonemap_shader};

	resource_manager::load_shader("ssao", "shaders/pbr/screen_quad_vertex.glsl", "shaders/ssao_fragment.glsl");
	resource_manager::load_shader("ssao_blur",
	                              "shaders/pbr/screen_quad_vertex.glsl",
	                              "shaders/ssao_blur_fragment.glsl");
}

aech::graphics::material_t aech::graphics::material_library::create_material(const std::string& from)
{
	auto it = default_materials.find(from);
	if (it != std::end(default_materials))
	{
		return (*it).second;
	}

	std::clog << "There is no " << from << " default material" << std::endl;
	return default_materials["default"];
}
