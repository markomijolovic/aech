#include "render_target.hpp"
#include <iostream>

//namespace aech
//{
//	render_target_t::render_target_t(uint32_t width,
//		uint32_t height,
//		GLenum type,
//		uint32_t nr_colour_attachments,
//		bool depth_and_stencil)
//		:m_height{height}, m_width{width}, m_type{type}, m_depth_and_stencil{depth_and_stencil}
//	{
//		glGenFramebuffers(1, &m_id);
//		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
//
//		for (size_t i =0; i < nr_colour_attachments; i++)
//		{
//			auto internal_format = type == GL_HALF_FLOAT ? GL_RGBA16F : type == GL_FLOAT ? GL_RGBA32F : GL_RGBA8;
//			texture_t texture{ width, height, internal_format, GL_RGBA, type, 0 };
//
//
//			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture.m_id, 0);
//			m_colour_attachments.emplace_back(texture);
//		}
//
//		if (depth_and_stencil)
//		{
//			m_depth_and_stencil_texture.m_filter_max = m_depth_and_stencil_texture.m_filter_min = GL_LINEAR;
//			m_depth_and_stencil_texture.m_wrap_s = m_depth_and_stencil_texture.m_wrap_t = GL_CLAMP_TO_EDGE;
//			m_depth_and_stencil_texture.m_mipmap = false;
//
//			m_depth_and_stencil_texture.generate(width, height, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL, type, 0);
//
//			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_and_stencil_texture.m_id, 0);
//		}
//
//		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		{
//			std::cerr << "Framebuffer not complete\n";
//		}
//
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	}
//
//
//	void render_target_t::resize(uint32_t width, uint32_t height)
//	{
//		m_width = width;
//		m_height = height;
//
//		for (auto &attachment: m_colour_attachments)
//		{
//			attachment.resize(width, height);
//		}
//
//		if (m_depth_and_stencil)
//		{
//			m_depth_and_stencil_texture.resize(width, height);
//		}
//	}
//}
