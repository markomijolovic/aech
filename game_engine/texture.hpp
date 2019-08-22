#pragma once
#include "types.hpp"
#include <GL/glew.h>

namespace aech
{
	class texture_t
	{
	public:
		uint32_t m_id{};
		GLenum m_target = GL_TEXTURE_2D;
		GLenum m_internal_format = GL_RGBA;
		GLenum m_format = GL_RGBA;
		GLenum m_type = GL_UNSIGNED_BYTE;
		GLenum m_filter_min = GL_LINEAR_MIPMAP_LINEAR;
		GLenum m_filter_max = GL_LINEAR;
		GLenum m_wrap_s = GL_REPEAT;
		GLenum m_wrap_t = GL_REPEAT;
		GLenum m_wrap_r = GL_REPEAT;
		bool m_mipmap = true;

		uint32_t m_width{};
		uint32_t m_height{};
		uint32_t m_depth{};

		texture_t() = default;
		~texture_t() = default;

		void generate(uint32_t width, GLenum internal_format, GLenum format, GLenum type, void* data);
		void generate(uint32_t width, uint32_t height, GLenum internal_format, GLenum format, GLenum type, void* data);
		void generate(uint32_t width, uint32_t height, uint32_t depth, GLenum internal_format, GLenum format, GLenum type, void* data);
		void bind(int32_t = -1);
		void unbind();
		void resize(uint32_t width, uint32_t height = 0, uint32_t depth = 0);
	};
}
