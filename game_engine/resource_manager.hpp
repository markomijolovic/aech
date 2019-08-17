#pragma once
#include "shader.hpp"
#include <string>
#include <unordered_map>

namespace aech::resource_manager
{	
	static inline std::unordered_map<std::string, shader_t> shaders;
	
	shader_t& load_shader(
		const std::string& vertex,
		const std::string& fragment,
		const std::string& geometry,
		const std::string& name
	);

	shader_t& get_shader(const std::string& name);

}
