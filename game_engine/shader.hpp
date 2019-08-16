#pragma once

#include "GL/glew.h"

#include "vec3.hpp"
#include "mat4.hpp"

#include <string>

namespace aech
{
	class shader_t
	{
	public:
		shader_t(const std::string& vertex_path, const std::string& fragment_path);
		void use() const;

		template<typename T>
		void set_uniform(const std::string &name, const T&value)
		{
			if constexpr(std::is_same_v<T, mat4_t>) {
				glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_TRUE, (GLfloat *)value.data);
			}
			else if constexpr(std::is_same_v<T, vec3_t>) {
				glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, (GLfloat *)&value);
			}
		}
	private:
		GLuint m_id;
	};
}
