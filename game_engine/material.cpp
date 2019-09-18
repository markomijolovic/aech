#include "material.hpp"
#include "mat4.hpp"

#include "shader.hpp"

#include "texture.hpp"

#include "texture_cube.hpp"


// TODO(Marko): Look into using fewer glUseProgram calls
namespace aech::graphics
{
	const std::unordered_map<std::string, std::pair<const texture_t*, uint32_t>>& material_t::get_textures() const
	{
		return m_textures;
	}

	const texture_t* material_t::get_texture(const std::string& name)
	{
		if (m_textures.find(name) != std::end(m_textures))
		{
			return m_textures[name].first;
		}
		return nullptr;
	}

	const texture_cube_t* material_t::get_texture_cube(const std::string& name)
	{
		if (m_texture_cubes.find(name) != std::end(m_texture_cubes))
		{
			return m_texture_cubes[name].first;
		}
		return nullptr;
	}

	material_t::material_t(shader_t* m_shader, material_type m_type) :
		m_shader{m_shader},
		m_type{m_type}
	{
	}

	shader_t* material_t::shader() const
	{
		return m_shader;
	}

	material_t::material_type material_t::type() const
	{
		return m_type;
	}

	void material_t::set_texture(const std::string& name, const texture_t* texture, uint32_t unit)
	{
		m_textures[name] = {texture, unit};
	}


	void material_t::set_texture_cube(const std::string& name, const texture_cube_t* texture, uint32_t unit)
	{
		m_texture_cubes[name] = {texture, unit};
	}

	void material_t::set_uniforms() const
	{
		m_shader->use();

		for (auto& el : m_textures)
		{
			el.second.first->bind(el.second.second);
			m_shader->set_uniform(el.first, el.second.second);
		}

		for (auto& el : m_texture_cubes)
		{
			el.second.first->bind(el.second.second);
			m_shader->set_uniform(el.first, el.second.second);
		}

		for (auto& el : m_uniforms)
		{
			if (el.second.type() == typeid(math::mat4_t))
			{
				m_shader->set_uniform(el.first, std::any_cast<math::mat4_t>(el.second));
			}
			else if (el.second.type() == typeid(math::vec3_t))
			{
				m_shader->set_uniform(el.first, std::any_cast<math::vec3_t>(el.second));
			}
			else if (el.second.type() == typeid(math::vec4_t))
			{
				m_shader->set_uniform(el.first, std::any_cast<math::vec4_t>(el.second));
			}
			else if (el.second.type() == typeid(bool))
			{
				m_shader->set_uniform(el.first, std::any_cast<bool>(el.second));
			}
			else if (el.second.type() == typeid(int32_t))
			{
				m_shader->set_uniform(el.first, std::any_cast<int32_t>(el.second));
			}
			else if (el.second.type() == typeid(uint32_t))
			{
				m_shader->set_uniform(el.first, std::any_cast<uint32_t>(el.second));
			}
			else if (el.second.type() == typeid(float))
			{
				m_shader->set_uniform(el.first, std::any_cast<float>(el.second));
			} // namespace aech::graphics
		}
	}
}
