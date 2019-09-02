#include "render_target.hpp"
#include <iostream>

namespace aech
{
	render_target_t::render_target_t(uint32_t width,
		uint32_t height,
		GLenum type,
		uint32_t nr_colour_attachments,
		bool depth_and_stencil)
		:m_width{width}, m_height{height}, m_type{type}, m_depth_and_stencil{depth_and_stencil}
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		auto internal_format = type == GL_HALF_FLOAT ? GL_RGBA16F : type == GL_FLOAT ? GL_RGBA32F : GL_RGBA8;

		for (size_t i =0; i < nr_colour_attachments; i++)
		{
			m_colour_attachments.emplace_back(width, height, internal_format, GL_RGBA, type, nullptr, false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colour_attachments.back().m_id, 0);
		}

		if (depth_and_stencil)
		{
			m_depth_and_stencil_texture = std::make_unique<texture_t>(width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr, false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_and_stencil_texture->m_id, 0);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Framebuffer not complete\n";
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void render_target_t::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void render_target_t::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
