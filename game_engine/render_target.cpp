#include "render_target.hpp"
#include <iostream>

namespace aech::graphics
{
	render_target_t::render_target_t(uint32_t width,
		uint32_t height,
		uint32_t num_colour_attachments,
		bool depth,
		texture_types::sized_internal_format sized_internal_format,
		texture_types::format format,
		texture_types::type type)
		: width{width}, height{height}
	{
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		for (size_t i = 0; i < num_colour_attachments; i++)
		{
			m_colour_attachments.emplace_back();
			m_colour_attachments.back().width = width;
			m_colour_attachments.back().height = height;
			m_colour_attachments.back().sized_internal_format = sized_internal_format;
			m_colour_attachments.back().format = format;
			m_colour_attachments.back().type = type;
			m_colour_attachments.back().mipmap = false;
			m_colour_attachments.back().filtering_mag = texture_types::filtering::nearest;
			m_colour_attachments.back().filtering_min = texture_types::filtering::nearest;
			m_colour_attachments.back().mipmap = false;
			m_colour_attachments.back().generate();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colour_attachments.back().id, 0);
		}

		if (depth)
		{
			m_depth_and_stencil_texture = std::make_unique<texture_t>();
			m_depth_and_stencil_texture->width = width;
			m_depth_and_stencil_texture->height = height;
			m_depth_and_stencil_texture->sized_internal_format = texture_types::sized_internal_format::depth24;
			m_depth_and_stencil_texture->format = texture_types::format::depth;
			m_depth_and_stencil_texture->type = texture_types::type::floating_point;
			m_colour_attachments.back().filtering_mag = texture_types::filtering::nearest;
			m_colour_attachments.back().filtering_min = texture_types::filtering::nearest;
			m_depth_and_stencil_texture->mipmap = false;
			m_depth_and_stencil_texture->generate();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_and_stencil_texture->id, 0);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Framebuffer not complete\n";
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void render_target_t::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void render_target_t::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
