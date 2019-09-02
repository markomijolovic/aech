#include "material_library.hpp"
#include "resource_manager.hpp"
#include <iostream>

void aech::material_library::generate_default_materials()
{
	auto default_shader = &resource_manager::load_shader("shaders/g_buffer_vertex.glsl", "shaders/g_buffer_fragment.glsl", "", "default");
	auto& default_material = default_materials["default"];
	default_material.m_shader = default_shader;
	//TODO: set default material textures

	auto directional = &resource_manager::load_shader("shaders/directional_vertex.glsl", "shaders/directional_fragment.glsl", "", "directional");
	auto& directional_material = default_materials["directional"];
	directional_material.m_shader = directional;
	// TODO: look into using set_texture
	directional_material.set_uniform("texture_position", 0);
	directional_material.set_uniform("texture_normal", 1);
	directional_material.set_uniform("texture_albedo", 2);
	directional_material.set_uniform("texture_metallic_roughness_ao", 3);

	auto point = &resource_manager::load_shader("shaders/point_vertex.glsl", "shaders/point_fragment.glsl", "", "point");
	auto& point_material = default_materials["point"];
	point_material.m_shader = point;
	point_material.set_uniform("texture_position", 0);
	point_material.set_uniform("texture_normal", 1);
	point_material.set_uniform("texture_albedo", 2);
	point_material.set_uniform("texture_metallic_roughness_ao", 3);
}

material_t material_library::create_material(const std::string& from)
{
	auto it = default_materials.find(from);
	if (it != std::end(default_materials))
	{
		return (*it).second;
	}

	std::cerr << "There is no " << from << " default material" << std::endl;
	return material_t{};
}
