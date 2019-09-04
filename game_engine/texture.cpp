#include "texture.hpp"
#include <algorithm>

// TODO: refactor generate function
// TODO: default arguments for height and depth?
namespace aech
{
	texture_t::texture_t(uint32_t width, uint32_t height, GLenum internal_format, GLenum format, GLenum type, void *data, bool mipmap)
	{
		glGenTextures(1, &m_id);

		bind();
		// calculate max number of mips
		const int levels =  mipmap?floor(log2(std::max(width, height))) + 1: 1;
		// only 2D textures for now
		glTexStorage2D(GL_TEXTURE_2D, levels, internal_format, width, height);
		//TODO: test
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap? GL_LINEAR_MIPMAP_LINEAR: GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (data)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, data);
		}
		if (mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		unbind();
	}


	void texture_t::bind(int32_t unit) const
	{
		if (unit >= 0)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
		}

		glBindTexture(GL_TEXTURE_2D, m_id);
	}


	void texture_t::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
