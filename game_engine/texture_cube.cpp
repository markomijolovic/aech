#include "texture_cube.hpp"
#include <algorithm>
#include <glad/glad.h>
#include <ostream>
#include <iostream>

namespace aech::graphics
{
	void texture_cube_t::init()
	{
		glGenTextures(1, &id);

		bind();
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filtering_min));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filtering_mag));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrap_s));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrap_t));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLenum>(wrap_r));
		const int levels = mipmap ? floor(log2(std::max(width, height))) + 1 : 1;
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, levels, static_cast<GLenum>(sized_internal_format), width, height);
		unbind();
	}

	void texture_cube_t::bind(int32_t unit) const
	{
		if (unit >= 0)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
		}
		glBindTexture(static_cast<GLenum>(target), id);
	}

	void texture_cube_t::unbind() const
	{
		glBindTexture(static_cast<GLenum>(target), 0);
	}


	void texture_cube_t::generate_mips()
	{
		bind();
		glGenerateMipmap(static_cast<GLenum>(target));
		unbind();
	}


	void texture_cube_t::generate_face(uint32_t index,
		void* data)
	{
		bind();
		
		if (data != nullptr)
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, 0, 0, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), data);
		}
		unbind();
	}
}
