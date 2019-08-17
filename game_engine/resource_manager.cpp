#include "resource_manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "stb_image.hpp"

aech::shader_t& aech::resource_manager::load_shader(const std::string& vertex,
	const std::string& fragment,
	const std::string& geometry,
	const std::string& name)
{
	if (shaders.find(name) != std::end(shaders))
	{
		return shaders[name];
	}

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


aech::texture_t& aech::resource_manager::load_texture(const std::string& name,
	const std::string& path,
	GLenum target,
	GLenum format,
	bool srgb)
{
	if (textures.find(name) != std::end(textures))
	{
		return textures[name];
	}

	texture_t texture{};
	texture.m_target = target;
	texture.m_internal_format = format;
	if (texture.m_internal_format == GL_RGB || texture.m_internal_format == GL_SRGB)
	{
		texture.m_internal_format = srgb ? GL_SRGB : GL_RGB;
	}

	if (texture.m_internal_format == GL_RGBA || texture.m_internal_format == GL_SRGB_ALPHA)
	{
		texture.m_internal_format = srgb ? GL_SRGB_ALPHA : GL_SRGB;
	}

	// make opengl happy
	stbi_set_flip_vertically_on_load(true);

	int32_t width, height, number_of_components;
	auto data = stbi_load(path.c_str(), &width, &height, &number_of_components, 0);

	if (data)
	{
		GLenum format{};
		switch(number_of_components)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default: 
			//do nothing;
			break;
		}

		if (target == GL_TEXTURE_1D)
		{
			texture.generate(width, texture.m_internal_format, format, GL_UNSIGNED_BYTE, data);
		}
		else if (target == GL_TEXTURE_2D)
		{
			texture.generate(width,height, texture.m_internal_format, format, GL_UNSIGNED_BYTE, data);
		}
	}
	else
	{
		std::cerr << "Failed to load texture at: " << path << '\n';
	}
	stbi_image_free(data);

	texture.m_width = width;
	texture.m_height = height;

	return textures[name] = texture;
}

aech::texture_t& aech::resource_manager::get_texture(const std::string& name)
{
	return textures[name];
}


aech::texture_t& aech::resource_manager::load_hdr_texture(const std::string& name, const std::string& path)
{
	if (textures.find(name) != std::end(textures))
	{
		return textures[name];
	}

	texture_t texture{};
	texture.m_target = GL_TEXTURE_2D;
	texture.m_filter_min = GL_LINEAR;
	texture.m_mipmap = false;

	stbi_set_flip_vertically_on_load(true);

	if (stbi_is_hdr(path.c_str()))
	{
		int32_t width, height, number_of_components;
		auto data = stbi_loadf(path.c_str(), &width, &height, &number_of_components, 0);

		if (data)
		{
			GLenum internal_format, format;
			if (number_of_components == 3)
			{
				internal_format = GL_RGB32F;
				format = GL_RGB;
			}
			else if (number_of_components == 4)
			{
				internal_format = GL_RGBA32F;
				format = GL_RGBA;
			}

			texture.generate(width, height, internal_format, format, GL_FLOAT, data);
			stbi_image_free(data);
		}

		texture.m_width = width;
		texture.m_height = height;
	}
	else
	{
		std::cerr << "Failed to load HDR texture at: " << path << '\n';
	}

	return textures[name] = texture;
}
