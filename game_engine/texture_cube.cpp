#include "texture_cube.hpp"

namespace aech::graphics
{
	void texture_cube_t::init(uint32_t width, uint32_t height, GLenum format, GLenum type, bool mipmap)
	{
		glGenTextures(1, &m_id);

		m_face_width = width;
		m_face_height = height;
		m_format = format;
		m_type = type;
		m_mipmap = mipmap;

		if (type == GL_HALF_FLOAT && format == GL_RGB)
		{
			m_internal_format = GL_RGB16F;
		}
		else if (type == GL_FLOAT && format == GL_RGB )
		{
			m_internal_format = GL_RGB32F;
		}
		else if (type == GL_HALF_FLOAT && format == GL_RGBA )
		{
			m_internal_format = GL_RGBA16F;
		}
		else if (type == GL_FLOAT && format == GL_RGBA)
		{
			m_internal_format = GL_RGBA32F;
		}

		bind();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_filter_min);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_filter_max);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrap_s);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrap_t);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrap_r);
	
		for (size_t i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_internal_format, m_face_width, m_face_height, 0, m_format, m_type, nullptr);
		}

		if (m_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}


	void texture_cube_t::generate_face(GLenum face,
		uint32_t width,
		uint32_t height,
		GLenum format,
		GLenum type,
		void* data)
	{
		if (m_face_width == 0)
		{
			glGenTextures(1, &m_id);
		}

		m_face_width = width;
		m_face_height = height;
		m_format = format;
		m_type = type;

		bind();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_filter_min);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_filter_max);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrap_s);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrap_t);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrap_r);


		glTexImage2D(face, 0, format, width, height, 0, format, type, data);
	}

	void texture_cube_t::bind(int unit)
	{
		if (unit >= 0)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
	}

	void texture_cube_t::unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

}
