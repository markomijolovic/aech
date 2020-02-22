#pragma once
#include "aech_types.hpp"

#include "mesh.hpp"

#include "shader.hpp"

#include "texture.hpp"

#include "texture_cube.hpp"

#include <assimp/material.h>

#include <string>

#include <unordered_map>
#include "material.hpp"


struct aiMesh;
struct aiNode;
struct aiScene;

namespace aech::resource_manager
{
	// shader storage, identified by string name
	inline std::unordered_map<std::string, graphics::shader_t> shaders{};

	// texture storage, identified by string name
	inline std::unordered_map<std::string, graphics::texture_t> textures{};

	// cubemap texture storage, identified by string name
	inline std::unordered_map<std::string, graphics::texture_cube_t> texture_cubes{};

	// these 2 are only used during loading process
	// to avoid duplicating meshes and materials.
	// after the loading process the pointers are actually destroyed
	// but the mesh filter components maintans the pointers to the 
	// referenced meshes and materials
	inline std::unordered_map<aiMesh*, graphics::mesh_t> meshes{};
	inline std::unordered_map<aiMaterial*, graphics::material_t> materials{};

	entity_t load_mesh(const std::string& path);
	entity_t process_node(const aiNode* node, const aiScene* scene);
	graphics::mesh_t* parse_mesh(aiMesh* mesh, const aiScene* scene);

	// load the given vertex, fragment and geometry shaders
	// and create a shader object identified by the given string name
	graphics::shader_t& load_shader(
		const std::string& name,
		const std::string& vertex,
		const std::string& fragment,
		const std::string& geometry = {}
	);

	// create a texture object identified by the given string name
	// path contains an LDR image
	graphics::texture_t* load_texture(const std::string& name, const std::string& path, bool srgb = true);

	// create a texture object identified by the given string name
	// path contains an HDR image
	graphics::texture_t* load_hdr_texture(const std::string& name, const std::string& path);

	// create a cubemap texture identified by the given stirng name
	graphics::texture_cube_t* load_texture_cube(const std::string& name,
	                                            const std::string& top,
	                                            const std::string& bottom,
	                                            const std::string& left,
	                                            const std::string& right,
	                                            const std::string& front,
	                                            const std::string& back,
	                                            bool srgb = true);

	// create a cubemap texture identified by the given string name
	graphics::texture_cube_t* load_texture_cube(const std::string& name, const std::string& folder);

	// create a material object from the given aiMaterial (Assimp class)
	graphics::material_t* parse_material(const aiScene* scene, aiMaterial* material);
} // namespace aech::resource_manager
