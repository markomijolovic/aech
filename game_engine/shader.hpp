#pragma once

#include <glad/glad.h>

#include "vec3.hpp"
#include "mat4.hpp"

#include <string>

namespace aech
{
	class shader_t
	{
	public:
		shader_t() = default;
		void use() const;
		void compile(
			const std::string& vertex_source,
			const std::string& fragment_source,
			const std::string& geometry_source
		);

		template<typename T>
		void set_uniform(const std::string &name, const T&value)
		{
			if constexpr(std::is_same_v<T, mat4_t>) {
				glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, true, (GLfloat *)(value.data));
			}
			else if constexpr(std::is_same_v<T, vec3_t>) {
				glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, (GLfloat*)(&value));
			}
			else if constexpr(std::is_same_v<T, vec4_t>) {
				glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, (GLfloat*)(&value));
			}
			else if constexpr(std::is_same_v<T, bool>) {
				glUniform1i(glGetUniformLocation(m_id, name.c_str()), (GLint)(value));
			}
			else if constexpr(std::is_same_v<T, int32_t>) {
				glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
			}
			else if constexpr(std::is_same_v<T, uint32_t>) {
				glUniform1i(glGetUniformLocation(m_id, name.c_str()), static_cast<int32_t>(value));
			}
			else if constexpr(std::is_same_v<T, float>) {
				glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
			}
		}
	private:
		uint32_t m_id;
		static void check_compile_errors(uint32_t id, const std::string &type);
	};
}
