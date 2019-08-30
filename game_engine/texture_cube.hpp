#pragma once

#include "types.hpp"
#include <glad/glad.h>

namespace aech
{
	class texture_cube_t
	{
	public:
		uint32_t m_id;

		GLenum m_internal_format = GL_RGBA;
		GLenum m_format = GL_RGBA;
		GLenum m_type = GL_UNSIGNED_BYTE;
		GLenum m_filter_min = GL_LINEAR;
		GLenum m_filter_max = GL_LINEAR;
		GLenum m_wrap_s = GL_CLAMP_TO_EDGE;
		GLenum m_wrap_t = GL_CLAMP_TO_EDGE;
		GLenum m_wrap_r = GL_CLAMP_TO_EDGE;
		bool m_mipmap = false;

		uint32_t m_face_width{};
		uint32_t m_face_height{};

		void bind(int32_t unit = -1);
		void unbind();
		//default init faces
		void init(uint32_t width, uint32_t height, GLenum format, GLenum type, bool mipmap);
		void generate_face(GLenum face, uint32_t width, uint32_t height, GLenum format, GLenum type, void* data);
	};
}
