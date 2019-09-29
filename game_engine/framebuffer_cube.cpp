#include "framebuffer_cube.hpp"

aech::graphics::framebuffer_cube_t::framebuffer_cube_t(texture_cube_t* texture, uint32_t width, uint32_t height) :
	m_texture{texture},
	m_width{width},
	m_height{height}
{
	glGenFramebuffers(1, &id);
	glGenRenderbuffers(1, &rbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);
}

void aech::graphics::framebuffer_cube_t::attach(uint32_t i, uint32_t miplevel) const
{
	glFramebufferTexture2D(GL_FRAMEBUFFER,
	                       GL_COLOR_ATTACHMENT0,
	                       GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	                       m_texture->id(),
	                       miplevel);
}

void aech::graphics::framebuffer_cube_t::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}


aech::graphics::texture_cube_t* aech::graphics::framebuffer_cube_t::texture() const
{
	return m_texture;
}

void aech::graphics::framebuffer_cube_t::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t aech::graphics::framebuffer_cube_t::width() const
{
	return m_width;
}

uint32_t aech::graphics::framebuffer_cube_t::height() const
{
	return m_height;
}
