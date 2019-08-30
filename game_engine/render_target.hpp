#pragma once

#include "types.hpp"
#include <glad/glad.h>
#include "texture.hpp"
#include <vector>

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
		texture_t m_depth_and_stencil_texture{};
		std::vector<texture_t> m_colour_attachments{};

		render_target_t(uint32_t width, uint32_t height, GLenum type = GL_UNSIGNED_BYTE, uint32_t nr_colour_attachments = 1, bool depth_and_stencil = true);
		void resize(uint32_t width, uint32_t height);
	
	};
}
