#pragma once
#include "shader.hpp"
#include <string>
#include <unordered_map>
#include "texture.hpp"

namespace aech::resource_manager
{	
	static inline std::unordered_map<std::string, shader_t> shaders{};
	static inline std::unordered_map<std::string, texture_t> textures{};
	
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
