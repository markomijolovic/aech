#include "resource_manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

aech::shader_t& aech::resource_manager::load_shader(const std::string& vertex,
	const std::string& fragment,
	const std::string& geometry,
	const std::string& name)
{
	std::string vertex_code{};
	std::string fragment_code{};
	std::string geometry_code{};

	try
	{
		std::ifstream vertex_file{ vertex };
		std::ifstream fragment_file{ fragment };

		std::stringstream vertex_stream{};
		std::stringstream fragment_stream{};

		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();

		if (!geometry.empty())
		{
			std::ifstream     geometry_file{ geometry };
			std::stringstream geometry_stream{};

			geometry_stream << geometry_file.rdbuf();
			geometry_code = geometry_stream.str();
		}
	}
	catch (const std::exception& exc)
	{
		std::cerr << exc.what() << '\n';
	}

	shaders[name] = shader_t{};
	shaders[name].compile(vertex_code, fragment_code, geometry_code);
	return shaders[name];
}

aech::shader_t& aech::resource_manager::get_shader(const std::string& name)
{
	return shaders[name];
}
