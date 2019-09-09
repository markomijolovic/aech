#include "framebuffer_cube.hpp"

aech::graphics::framebuffer_cube_t::framebuffer_cube_t(texture_cube_t* texture, uint32_t width, uint32_t height)
	: texture{texture}, width {width}, height{height}
{
	glGenFramebuffers(1, &id);
	glGenRenderbuffers(1, &rbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);
}

void aech::graphics::framebuffer_cube_t::attach(int i) const
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture->id, 0);
}

void aech::graphics::framebuffer_cube_t::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void aech::graphics::framebuffer_cube_t::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
