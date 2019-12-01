#pragma once
#include "aech_types.hpp"

#include "mesh.hpp"

#include "scene_node.hpp"

#include "shader.hpp"

#include "texture.hpp"

#include "texture_cube.hpp"

#include <assimp/material.h>

#include <string>

#include <unordered_map>


struct aiMesh;
struct aiNode;
struct aiScene;

namespace aech::resource_manager
{
	inline std::unordered_map<std::string, graphics::shader_t>       shaders{};
	inline std::unordered_map<std::string, graphics::texture_t>      textures{};
	inline std::unordered_map<std::string, graphics::texture_cube_t> texture_cubes{};

	// these 2 are only used during loading process
	// to avoid duplicating meshes and materials.
	// after the loading process the pointers are actually destroyed
	// but the mesh filter components maintans the pointers to the 
	// referenced meshes and materials
	inline std::unordered_map<aiMesh*, graphics::mesh_t>         meshes{};
	inline std::unordered_map<aiMaterial*, graphics::material_t> materials{};

	entity_t          load_mesh(const std::string& path);
	entity_t          process_node(const aiNode* node, const aiScene* scene);
	graphics::mesh_t* parse_mesh(aiMesh* mesh, const aiScene* scene);

	graphics::shader_t& load_shader(
		const std::string& name,
		const std::string& vertex,
		const std::string& fragment,
		const std::string& geometry = {}
	);
	graphics::texture_t*      load_texture(const std::string& name, const std::string& path, bool srgb = true);
	graphics::texture_t*      load_hdr_texture(const std::string& name, const std::string& path);
	graphics::texture_cube_t* load_texture_cube(const std::string& name,
	                                            const std::string& top,
	                                            const std::string& bottom,
	                                            const std::string& left,
	                                            const std::string& right,
	                                            const std::string& front,
	                                            const std::string& back,
	                                            bool               srgb = true);
	graphics::texture_cube_t* load_texture_cube(const std::string& name, const std::string& folder);
	graphics::material_t*     parse_material(const aiScene* scene, aiMaterial* material);
} // namespace aech::resource_manager
