#pragma once
#include "shader.hpp"
#include <string>
#include <unordered_map>
#include "texture.hpp"
#include "mesh.hpp"
#include "scene_node.hpp"
#include <optional>

struct aiMesh;
struct aiNode;
struct aiScene;

namespace aech::resource_manager
{	
	inline std::unordered_map<std::string, shader_t> shaders{};
	inline std::unordered_map<std::string, texture_t> textures{};
	inline std::vector<mesh_t*> meshes{};

	entity_t load_mesh(const std::string& path);
	entity_t process_node(const aiNode* node, const aiScene* scene);
	std::unique_ptr<mesh_t> parse_mesh(aiMesh* mesh, const aiScene* scene);

	shader_t& load_shader(
		const std::string& vertex,
		const std::string& fragment,
		const std::string& geometry,
		const std::string& name
	);
	shader_t& get_shader(const std::string& name);
	texture_t& load_texture(const std::string& name, const std::string& path, GLenum target, GLenum format, bool srgb);
	texture_t& load_hdr_texture(const std::string& name, const std::string& path);
	texture_t& get_texture(const std::string& name);
}
