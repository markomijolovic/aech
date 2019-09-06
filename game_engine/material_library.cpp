#include "material_library.hpp"
#include "resource_manager.hpp"
#include <iostream>

void aech::graphics::material_library::generate_default_materials()
{
	auto default_shader = &resource_manager::load_shader("default", "shaders/g_buffer_vertex.glsl", "shaders/g_buffer_fragment.glsl");
	auto& default_material = default_materials["default"];
	default_material.m_shader = default_shader;
	//TODO: set default material textures

	auto directional = &resource_manager::load_shader("directional", "shaders/directional_vertex.glsl", "shaders/directional_fragment.glsl");
	auto& directional_material = default_materials["directional"];
	directional_material.m_shader = directional;
	// TODO: look into using set_texture
	directional_material.set_uniform("texture_position", 0);
	directional_material.set_uniform("texture_normal", 1);
	directional_material.set_uniform("texture_albedo", 2);
	directional_material.set_uniform("texture_metallic_roughness_ao", 3);
	directional_material.set_uniform("light_shadow_map", 4);

	auto point = &resource_manager::load_shader("point", "shaders/point_vertex.glsl", "shaders/point_fragment.glsl");
	auto& point_material = default_materials["point"];
	point_material.m_shader = point;
	point_material.set_uniform("texture_position", 0);
	point_material.set_uniform("texture_normal", 1);
	point_material.set_uniform("texture_albedo", 2);
	point_material.set_uniform("texture_metallic_roughness_ao", 3);

	auto shadow_shader = &resource_manager::load_shader("shadow", "shaders/shadow_vertex.glsl", "shaders/shadow_fragment.glsl");
	auto& shadow_material = default_materials["shadow"];
	shadow_material.m_shader = shadow_shader;

	auto opaque_shader = &resource_manager::load_shader("transparent", "shaders/transparent_vertex.glsl", "shaders/transparent_fragment.glsl");
	auto& opaque_material = default_materials["transparent"];
	opaque_material.set_uniform("texture_albedo", 0);
	opaque_material.set_uniform("texture_normal", 1);
	opaque_material.set_uniform("texture_metallic", 2);
	opaque_material.set_uniform("texture_roughness", 3);
	opaque_material.set_uniform("light_shadow_map", 4);
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
