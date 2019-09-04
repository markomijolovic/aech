#pragma once

#include "types.hpp"
#include <glad/glad.h>
#include "texture.hpp"
#include <vector>
#include <memory>

namespace aech
{
	class render_target_t
	{
	public:
		uint32_t m_id;
		uint32_t m_width;
		uint32_t m_height;
		GLenum m_type;

		bool m_depth_and_stencil;

		GLenum m_target = GL_TEXTURE_2D;
		std::unique_ptr<texture_t> m_depth_and_stencil_texture{};
		std::vector<texture_t> m_colour_attachments{};

		// this needs refactoring
		render_target_t(uint32_t width, uint32_t height, GLenum type = GL_FLOAT, uint32_t nr_colour_attachments = 1, bool depth_and_stencil = true, GLenum internal_format = GL_RGBA8, GLenum format = GL_RGBA);
		void bind() const;
		void unbind() const;
	};
}
