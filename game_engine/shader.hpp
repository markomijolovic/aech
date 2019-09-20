#pragma once
#include "glad/glad.h"

#include "mat4.hpp"

#include "vec3.hpp"


#include <string>

namespace aech::graphics
{
	class shader_t
	{
	public:
		// make unordered_map happy
		shader_t() = default;
		shader_t(
			const std::string& vertex_source,
			const std::string& fragment_source,
			const std::string& geometry_source
		);
		void use() const;

		template <typename T>
		void set_uniform(const std::string& name, const T& value) const
		{
			if constexpr (std::is_same_v<T, math::mat4_t>)
			{
				glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, true, (GLfloat *)(value[0]));
			}
			else if constexpr (std::is_same_v<T, math::vec3_t>)
			{
				glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, (GLfloat*)(&value));
			}
			else if constexpr (std::is_same_v<T, math::vec4_t>)
			{
				glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, (GLfloat*)(&value));
			}
			else if constexpr (std::is_same_v<T, bool>)
			{
				glUniform1i(glGetUniformLocation(id, name.c_str()), (GLint)(value));
			}
			else if constexpr (std::is_same_v<T, int32_t>)
			{
				glUniform1i(glGetUniformLocation(id, name.c_str()), value);
			}
			else if constexpr (std::is_same_v<T, uint32_t>)
			{
				glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int32_t>(value));
			}
			else if constexpr (std::is_same_v<T, float>)
			{
				glUniform1f(glGetUniformLocation(id, name.c_str()), value);
			}
		}

	private:
		uint32_t    id;
		static void check_compile_errors(uint32_t id, const std::string& type);
	};
} // namespace aech::graphics
