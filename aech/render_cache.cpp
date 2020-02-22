#include "render_cache.hpp"


aech::graphics::render_cache_t::render_cache_t()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void aech::graphics::render_cache_t::set_depth_test(bool enable)
{
	if (m_depth_test != enable)
	{
		if (m_depth_test = enable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
}

void aech::graphics::render_cache_t::set_depth_func(depth_func func)
{
	if (m_depth_func != func)
	{
		m_depth_func = func;
		glDepthFunc(static_cast<GLenum>(m_depth_func));
	}
}

void aech::graphics::render_cache_t::set_blend(bool enable)
{
	if (m_blend != enable)
	{
		if (m_blend = enable)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}
}

void aech::graphics::render_cache_t::set_blend(blend_func source, blend_func dest)
{
	if (m_blend_source != source || m_blend_dest != dest)
	{
		m_blend_source = source;
		m_blend_dest   = dest;
		glBlendFunc(static_cast<GLenum>(source), static_cast<GLenum>(dest));
	}
}

void aech::graphics::render_cache_t::set_cull(bool enable)
{
	if (m_cull != enable)
	{
		if (m_cull = enable)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
}

void aech::graphics::render_cache_t::set_cull_face(cull_face face)
{
	if (m_cull_face != face)
	{
		m_cull_face = face;
		glCullFace(static_cast<GLenum>(m_cull_face));
	}
}

void aech::graphics::render_cache_t::set_shader(shader_t* shader)
{
	if ((m_shader == nullptr) || *m_shader != *shader)
	{
		m_shader = shader;
		m_shader->use();
	}
}


void aech::graphics::render_cache_t::clear(graphics::clear bit)
{
	glClear(static_cast<GLenum>(bit));
}

void aech::graphics::render_cache_t::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	if (m_viewport_dimensions != std::array{x, y, width, height})
	{
		m_viewport_dimensions = {x, y, width, height};
		glViewport(x, y, width, height);
	}
}
