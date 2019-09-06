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

	auto shadow_shader = &resource_manager::load_shader("shadow", "shaders/shadow_vertex.glsl", "shaders/shadow_fragment.glsl");
	auto& shadow_material = default_materials["shadow"];
	shadow_material.m_shader = shadow_shader;

	auto transparent_shader = &resource_manager::load_shader("transparent", "shaders/transparent_vertex.glsl", "shaders/transparent_fragment.glsl");
	auto& transparent_material = default_materials["transparent"];
	transparent_material.m_shader = transparent_shader;
}

aech::graphics::material_t aech::graphics::material_library::create_material(const std::string& from)
{
	auto it = default_materials.find(from);
	if (it != std::end(default_materials))
	{
		return (*it).second;
	}

	std::cerr << "There is no " << from << " default material" << std::endl;
	return material_t{};
}
