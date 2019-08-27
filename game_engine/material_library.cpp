#include "material_library.hpp"
#include "resource_manager.hpp"

void aech::material_library::generate_default_materials()
{
	auto default_shader = &resource_manager::load_shader("shaders/deferred_vertex.glsl", "shaders/deferred_fragment.glsl", "", "default");
	auto &default_material = default_materials["default"] = material_t{.m_shader = default_shader};
	default_material.type = material_type_t::material_default;
	//default_material.set_uniform("colour", vec4_t{  .0f, .5f, .75f, 1.0f } );
	//default_material.set_texture("diffuse", &resource_manager::load_texture("default_diffuse", "textures/default_diffuse.jpg", GL_TEXTURE_2D, GL_RGB, false), 0);
	//default_material.set_texture("albedo_texture", &resource_manager::load_texture("default_albedo", "textures/default_albedo.png", GL_TEXTURE_2D, GL_RGB, true), 3);
	//default_material.set_texture("normal_texture", &resource_manager::load_texture("default_normal", "textures/default_normal.png", GL_TEXTURE_2D, GL_RGBA, false), 4);
	//default_material.set_texture("metallic_texture", &resource_manager::load_texture("default_metallic", "textures/default_metallic.png", GL_TEXTURE_2D, GL_RGBA, false), 5);
	//default_material.set_texture("roughness_texture", &resource_manager::load_texture("default_roughness", "textures/default_roughness.png", GL_TEXTURE_2D, GL_RGBA, false), 6);
}
