#include "texture.hpp"

namespace aech
{
	void texture_t::generate(uint32_t width, GLenum internal_format, GLenum format, GLenum type, void* data)
	{
		glGenTextures(1, &m_id);
		m_width = width;
		m_internal_format = internal_format;
		m_format = format;
		m_type = type;

		//m_target == GL_TEXTURE_1D
		glTexImage1D(m_target, 0, m_internal_format, m_width, 0, m_format, m_type, data);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_filter_min);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_filter_max);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrap_s);
		if (m_mipmap)
			glGenerateMipmap(m_target);

	}

	void texture_t::generate(uint32_t width,
		uint32_t height,
		GLenum internal_format,
		GLenum format,
		GLenum type,
		void* data)
	{
		glGenTextures(1, &m_id);
		m_width = width;
		m_height = height;
		m_internal_format = internal_format;
		m_format = format;
		m_type = type;

		//m_target == GL_TEXTURE_2D
		glTexImage2D(m_target, 0, m_internal_format, m_width,m_height, 0, m_format, m_type, data);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_filter_min);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_filter_max);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrap_s);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_wrap_t);
		if (m_mipmap)
			glGenerateMipmap(m_target);

	}

	void texture_t::generate(uint32_t width,
		uint32_t height,
		uint32_t depth,
		GLenum internal_format,
		GLenum format,
		GLenum type,
		void* data)
	{
		glGenTextures(1, &m_id);
		m_width = width;
		m_height = height;
		m_depth = depth;
		m_internal_format = internal_format;
		m_format = format;
		m_type = type;

		//m_target == GL_TEXTURE_3D
		glTexImage3D(m_target, 0, m_internal_format, m_width, m_height,m_depth, 0, m_format, m_type, data);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_filter_min);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_filter_max);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, m_wrap_s);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, m_wrap_t);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, m_wrap_r);
		if (m_mipmap)
			glGenerateMipmap(m_target);
	}


	void texture_t::bind(int32_t unit)
	{
		if (unit >= 0)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
		}

		glBindTexture(m_target, m_id);
	}


	void texture_t::unbind()
	{
		glBindTexture(m_target, 0);
	}
}
