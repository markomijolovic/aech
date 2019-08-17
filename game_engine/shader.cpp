#include "shader.hpp"
#include <iostream>

namespace aech
{
	void shader_t::compile(
		const std::string& vertex_source,
		const std::string& fragment_source,
		const std::string& geometry_source)
	{
		auto s_vertex = glCreateShader(GL_VERTEX_SHADER);
		auto vertex_data = vertex_source.c_str();
		glShaderSource(s_vertex, 1, &vertex_data, nullptr);
		glCompileShader(s_vertex);
		check_compile_errors(s_vertex, "vertex");

		auto s_fragment = glCreateShader(GL_FRAGMENT_SHADER);
		auto fragment_data = fragment_source.c_str();
		glShaderSource(s_fragment, 1, &fragment_data, nullptr);
		glCompileShader(s_fragment);
		check_compile_errors(s_fragment, "fragment");

		uint32_t s_geometry;
		if (!geometry_source.empty())
		{
			s_geometry = glCreateShader(GL_GEOMETRY_SHADER);
			auto geometry_data = geometry_source.c_str();
			glShaderSource(s_geometry, 1, &geometry_data, nullptr);
			glCompileShader(s_geometry);
			check_compile_errors(s_geometry, "geometry");
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, s_vertex);
		glAttachShader(m_id, s_fragment);
		if (!geometry_source.empty())
		{
			glAttachShader(m_id, s_vertex);
		}

		glLinkProgram(m_id);
		check_compile_errors(m_id, "program");

		glDeleteShader(s_vertex);
		glDeleteShader(s_fragment);
		if (!geometry_source.empty())
		{
			glDeleteShader(s_geometry);
		}
	}

	void shader_t::use() const
	{
		glUseProgram(m_id);
	}


	void shader_t::check_compile_errors(uint32_t shader, const std::string& type)
	{
		GLint success;
		if (type == "program")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (success == 0)
			{
				GLchar info_log[1 << 10];
				glGetProgramInfoLog(shader, 1 << 10, nullptr, info_log);
				std::cerr << "ERROR::PROGRAM_LINKING_ERROR: " << info_log << '\n';
			}
		}
		else
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (success == 0)
			{
				GLchar info_log[1 << 10];
				glGetShaderInfoLog(shader, 1 << 10, nullptr, info_log);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR(" << type << ")\n" << info_log << '\n';
			}
		}
	}
}
