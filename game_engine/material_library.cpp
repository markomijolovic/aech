#include "material_library.hpp"
#include "resource_manager.hpp"
#include <iostream>

void aech::graphics::material_library::generate_default_materials()
{
	auto default_shader = &resource_manager::load_shader("default", "shaders/g_buffer_vertex.glsl", "shaders/g_buffer_fragment.glsl");
	auto& default_material = default_materials["default"];
	default_material.m_shader = default_shader;

	auto directional = &resource_manager::load_shader("directional", "shaders/directional_vertex.glsl", "shaders/directional_fragment.glsl");
	auto& directional_material = default_materials["directional"];
	directional_material.m_shader = directional;

	auto point = &resource_manager::load_shader("point", "shaders/point_vertex.glsl", "shaders/point_fragment.glsl");
	auto& point_material = default_materials["point"];
	point_material.m_shader = point;

	auto opaque_shadow_shader = &resource_manager::load_shader("opaque_shadow", "shaders/opaque_shadow_vertex.glsl", "shaders/opaque_shadow_fragment.glsl");
	auto& opaque_shadow_material = default_materials["opaque_shadow"];
	opaque_shadow_material.m_shader = opaque_shadow_shader;

	auto transparent_shadow_shader = &resource_manager::load_shader("transparent_shadow", "shaders/transparent_shadow_vertex.glsl", "shaders/transparent_shadow_fragment.glsl");
	auto& transparent_shadow_material = default_materials["transparent_shadow"];
	transparent_shadow_material.m_shader = transparent_shadow_shader;

	auto transparent_shader = &resource_manager::load_shader("transparent", "shaders/transparent_vertex.glsl", "shaders/transparent_fragment.glsl");
	auto& transparent_material = default_materials["transparent"];
	transparent_material.m_shader = transparent_shader;

	auto hdr_to_cubemap_shader = &resource_manager::load_shader("hdr_to_cubemap" , "shaders/pbr/cube_sample_vertex.glsl", "shaders/pbr/equirectangular_to_cube_fragment.glsl", "");
	auto& hdr_to_cubemap_material = default_materials["hdr_to_cubemap"];
	hdr_to_cubemap_material.m_shader = hdr_to_cubemap_shader;

	auto background_shader = &resource_manager::load_shader("background", "shaders/background_vertex.glsl", "shaders/background_fragment.glsl");

	auto irradiance_shader = &resource_manager::load_shader("precompute_irradiance", "shaders/pbr/cube_sample_vertex.glsl", "shaders/pbr/precompute_irradiance_fragment.glsl", "");

//
//	auto prefilter_shader = &resource_manager::load_shader("prefilter", "shaders/pbr/cube_sample_vertex.glsl", "shaders/pbr/prefilter_fragment.glsl", "");
//	auto& prefilter_material = default_materials["prefilter"];
//	prefilter_material.m_shader = prefilter_shader;
//
//	auto brdf_integral_shader = &resource_manager::load_shader("brdf_integral", "shaders/pbr/screen_quad_vertex.glsl", "shaders/pbr/brdf_integral_fragment.glsl", "");
//	auto& brdf_integral_material = default_materials["brdf_integral"];
//	brdf_integral_material.m_shader = brdf_integral_shader;
//
//	auto probe_capture_shader = &resource_manager::load_shader("probe_capture", "shaders/pbr/probe_capture_vertex.glsl", "shaders/pbr/probe_capture_fragment.glsl", "");
//	auto& probe_capture_material = default_materials["probe_capture"];
//	probe_capture_material.m_shader = probe_capture_shader;
//*/
//	// probe_render_shader
}

aech::graphics::material_t aech::graphics::material_library::create_material(const std::string& from)
{
	auto it = default_materials.find(from);
	if (it != std::end(default_materials))
	{
		return (*it).second;
	}

	std::clog << "There is no " << from << " default material" << std::endl;
	return material_t{};
}
