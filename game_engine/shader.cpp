#include "shader.hpp"
#include "iostream"
#include "fstream"
#include "sstream"

namespace aech
{
	shader_t::shader_t(const std::string& vertex_path, const std::string& fragment_path)
	{
		std::string fragment_file_contents;
		std::string vertex_file_contents;

		// Read in the vertex shader
		std::ifstream vertex_file;

		vertex_file.open(vertex_path);
		std::stringstream vertex_file_stream;
		vertex_file_stream << vertex_file.rdbuf();
		vertex_file.close();
		vertex_file_contents = vertex_file_stream.str();

		// Read in the fragment shader
		std::ifstream fragment_file;

		fragment_file.open(fragment_path);
		std::stringstream fragment_file_stream;
		fragment_file_stream << fragment_file.rdbuf();
		fragment_file.close();
		fragment_file_contents = fragment_file_stream.str();

		auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		auto vertex_shader_source = vertex_file_contents.c_str();
		glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
		glCompileShader(vertex_shader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
			std::cerr << "Error compiling vertex shader: " << infoLog << "\n";
		}


		auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		auto fragment_shader_source = fragment_file_contents.c_str();
		glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
		glCompileShader(fragment_shader);
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
			std::cerr << "Error compiling fragment shader: " << infoLog << "\n";
		}


		m_id = glCreateProgram();

		glAttachShader(m_id, vertex_shader);
		glAttachShader(m_id, fragment_shader);
		glLinkProgram(m_id);
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
			std::cerr << "Error linking shaders: " << infoLog << "\n";
		}

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	void shader_t::use() const
	{
		glUseProgram(m_id);
	}
}
