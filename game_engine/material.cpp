#include "material.hpp"
#include "texture_cube.hpp"

namespace aech
{
	void material_t::set_texture(const std::string& name, const texture_t& texture, uint32_t unit)
	{
		m_sampler_uniforms[name].unit = unit;
		m_sampler_uniforms[name].texture = texture;

		switch(texture.m_target)
		{
		case GL_TEXTURE_1D:
			m_sampler_uniforms[name].type = shader_type_t::shader_type_sampler_1d;
			break;
		case GL_TEXTURE_2D:
			m_sampler_uniforms[name].type = shader_type_t::shader_type_sampler_2d;
			break;
		case GL_TEXTURE_3D:
			m_sampler_uniforms[name].type = shader_type_t::shader_type_sampler_3d;
			break;
		default:
			// yikes
			break;
		}

		if (m_shader)
		{
			m_shader->use();
			m_shader->set_uniform(name, unit);
		}
	}


	void material_t::set_texture_cube(const std::string& name, const texture_cube_t& texture, uint32_t unit)
	{
		m_sampler_uniforms[name].unit = unit;
		m_sampler_uniforms[name].type = shader_type_t::shader_type_sampler_cube;
		m_sampler_uniforms[name].texture = texture;

		if (m_shader)
		{
			m_shader->use();
			m_shader->set_uniform(name, unit);
		}
	}
}
