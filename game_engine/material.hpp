#pragma once
#include <unordered_map>
#include <any>
#include "texture.hpp"
#include "shader.hpp"

namespace aech::graphics
{
	class texture_cube_t;

	struct material_t
	{
		shader_t* m_shader;
		std::unordered_map<std::string, std::any> m_uniforms{};
		std::unordered_map<std::string, std::pair<const texture_t *, uint32_t>> m_textures{};
		std::unordered_map<std::string, std::pair<const texture_cube_t*, uint32_t>> m_texture_cubes{};

		// stores the uniform value in the material
		// note that this does not actually set it in the shader
		template<typename T>
		void set_uniform(const std::string &name, const T& value)
		{
			m_uniforms[name] = value;
		}

		// actually set the uniforms in the shader
		void set_uniforms() const;
		void set_texture(const std::string& name, const texture_t* texture, uint32_t unit);
		void set_texture_cube(const std::string& name, const texture_cube_t* texture, uint32_t unit);
	};
}
