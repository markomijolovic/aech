#include "texture_cube.hpp"
#include <algorithm>
#include "glad/glad.h"
#include <ostream>

namespace aech::graphics
{
	texture_cube_t::texture_cube_t(uint32_t                             width,
	                               uint32_t                             height,
	                               texture_types::sized_internal_format sif,
	                               texture_types::format                f,
	                               texture_types::type                  t,
	                               bool                                 mipmap,
	                               texture_types::filtering             min,
	                               texture_types::filtering             mag) :
		m_width{width},
		m_height{height},
		m_sized_internal_format{sif},
		m_format{f},
		m_type{t},
		m_filtering_min{min},
		m_filtering_mag{mag}
	{
		glGenTextures(1, &m_id);

		bind();
		glTexParameteri(static_cast<GLenum>(texture_types::target::cube_map),
		                GL_TEXTURE_MIN_FILTER,
		                static_cast<GLenum>(m_filtering_min));
		glTexParameteri(static_cast<GLenum>(texture_types::target::cube_map),
		                GL_TEXTURE_MAG_FILTER,
		                static_cast<GLenum>(m_filtering_mag));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLenum>(m_wrap_s));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLenum>(m_wrap_t));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLenum>(m_wrap_r));
		const auto levels = mipmap ? static_cast<uint32_t>(floor(log2(std::max(width, height)))) + 1 : 1U;
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, levels, static_cast<GLenum>(m_sized_internal_format), width, height);
		unbind();
	}

	uint32_t texture_cube_t::id() const
	{
		return m_id;
	}

	void texture_cube_t::bind(int32_t unit) const
	{
		if (unit >= 0)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
		}
		glBindTexture(static_cast<GLenum>(texture_types::target::cube_map), m_id);
	}

	void texture_cube_t::unbind()
	{
		glBindTexture(static_cast<GLenum>(texture_types::target::cube_map), 0);
	}


	void texture_cube_t::generate_mips() const
	{
		bind();
		glGenerateMipmap(static_cast<GLenum>(texture_types::target::cube_map));
		unbind();
	}


	void texture_cube_t::generate_face(uint32_t index,
	                                   void*    data) const
	{
		bind();

		if (data != nullptr)
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
			                0,
			                0,
			                0,
			                m_width,
			                m_height,
			                static_cast<GLenum>(m_format),
			                static_cast<GLenum>(m_type),
			                data);
		}
		unbind();
		// namespace aech::graphics
	}
}
