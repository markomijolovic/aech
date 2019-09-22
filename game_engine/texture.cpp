#include "texture.hpp"
#include <algorithm>

#include <iostream>


namespace aech::graphics
{
	texture_t::texture_t(uint32_t width,
		uint32_t height,
		texture_types::sized_internal_format sized_internal_format,
		texture_types::format format,
		texture_types::type type,
		bool mipmap,
		texture_types::filtering filtering_min,
		texture_types::filtering filtering_mag,
		void* data)
		: m_width{width}, m_height{height}, m_sized_internal_format{sized_internal_format}, m_format{format}, m_type{type}, m_mipmap{mipmap}, m_filtering_mag{filtering_mag}, m_filtering_min{filtering_min}, m_data{data}
	{
		glGenTextures(1, &m_id);
		bind();

		const int levels = m_mipmap ? floor(log2(std::max(m_width, m_height))) + 1 : 1;
		glTexStorage2D(static_cast<GLenum>(texture_types::target::twod), levels, static_cast<GLenum>(m_sized_internal_format), m_width, m_height);
		glTexParameteri(static_cast<GLenum>(texture_types::target::twod), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(m_filtering_min));
		glTexParameteri(static_cast<GLenum>(texture_types::target::twod), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(m_filtering_mag));
		glTexParameteri(static_cast<GLenum>(texture_types::target::twod), GL_TEXTURE_WRAP_S, static_cast<GLenum>(m_wrap_s));
		glTexParameteri(static_cast<GLenum>(texture_types::target::twod), GL_TEXTURE_WRAP_T, static_cast<GLenum>(m_wrap_t));
		if (m_data)
		{
			glTexSubImage2D(static_cast<GLenum>(texture_types::target::twod),
			                0,
			                0,
			                0,
			                m_width,
			                m_height,
			                static_cast<GLenum>(m_format),
			                static_cast<GLenum>(m_type),
			                m_data);
		}
		if (m_mipmap)
		{
			glGenerateMipmap(static_cast<GLenum>(texture_types::target::twod));
		}
		unbind();
	}


	uint32_t texture_t::id() const
	{
		return m_id;
	}

	void texture_t::bind(int32_t unit) const
	{
		if (unit >= 0)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
		}

		glBindTexture(static_cast<GLenum>(texture_types::target::twod), m_id);
	}


	void texture_t::unbind()
	{
		glBindTexture(static_cast<GLenum>(texture_types::target::twod), 0);
	}
} // namespace aech::graphics
