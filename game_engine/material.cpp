#include "material.hpp"
#include "texture_cube.hpp"

// TODO: Look into using fewer glUseProgram calls
namespace aech
{
	void material_t::set_texture(const std::string& name, const texture_t* texture, uint32_t unit)
	{
		m_sampler_uniforms[name].unit = unit;
		m_sampler_uniforms[name].texture = texture;

		//switch(texture->m_target)
		//{
		//case GL_TEXTURE_1D:
		//	m_sampler_uniforms[name].type = shader_type_t::shader_type_sampler_1d;
		//	break;
		//case GL_TEXTURE_2D:
		//	m_sampler_uniforms[name].type = shader_type_t::shader_type_sampler_2d;
		//	break;
		//case GL_TEXTURE_3D:
		//	m_sampler_uniforms[name].type = shader_type_t::shader_type_sampler_3d;
		//	break;
		//default:
		//	// yikes
		//	break;
		//}

		m_shader->use();
		m_shader->set_uniform(name, unit);
	}


	void material_t::set_texture_cube(const std::string& name, const texture_cube_t* texture, uint32_t unit)
	{
		m_sampler_uniforms[name].unit = unit;
		m_sampler_uniforms[name].type = shader_type_t::shader_type_sampler_cube;
		m_sampler_uniforms[name].texture = texture;

		m_shader->use();
		m_shader->set_uniform(name, unit);
	}


	void material_t::bind_textures() const
	{
		m_shader->use();
		for (const auto&el: m_sampler_uniforms)
		{
			auto val = std::get<const texture_t *>(el.second.texture);
			val->bind(el.second.unit);
			m_shader->set_uniform(el.first, el.second.unit);
		}
	}
}
