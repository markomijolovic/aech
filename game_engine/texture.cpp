#include "texture.hpp"
#include <algorithm>
#include <glad/glad.h>

#include <iostream>


namespace aech::graphics
{
	void texture_t::generate()
	{
		glGenTextures(1, &id);
		bind();

		const int levels = mipmap ? floor(log2(std::max(width, height))) + 1 : 1;
		glTexStorage2D(static_cast<GLenum>(target), levels, static_cast<GLenum>(sized_internal_format), width, height);
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filtering_min));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filtering_mag));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrap_s));
		glTexParameteri(static_cast<GLenum>(target), GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrap_t));
		if (data != nullptr)
		{
			glTexSubImage2D(static_cast<GLenum>(target),
			                0,
			                0,
			                0,
			                width,
			                height,
			                static_cast<GLenum>(format),
			                static_cast<GLenum>(type),
			                data);
		}
		if (mipmap)
		{
			glGenerateMipmap(static_cast<GLenum>(target));
		}
		unbind();
	}

	void texture_t::bind(int32_t unit) const
	{
		if (unit >= 0)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
		}

		glBindTexture(static_cast<GLenum>(target), id);
	}


	void texture_t::unbind() const
	{
		glBindTexture(static_cast<GLenum>(target), 0);
	}
} // namespace aech::graphics
