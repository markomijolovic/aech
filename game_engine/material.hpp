#pragma once
#include "shader.hpp"
#include <unordered_map>
#include "shading_types.hpp"

namespace aech
{
	class texture_cube_t;

	enum class material_type_t
	{
		material_default ,
		material_custom,
		material_post_process
	};

	struct material_t
	{
		//shader
		shader_t* m_shader;
		std::unordered_map<std::string, uniform_value_t> m_uniforms{};
		std::unordered_map<std::string, uniform_value_sampler_t> m_sampler_uniforms{};
		material_type_t type{ material_type_t::material_custom };
		vec4_t colour{};

		//depth
		bool depth_test = true;
		bool depth_write = true;
		GLenum depth_compare = GL_LESS;

		//culling
		bool cull = true;
		GLenum cull_face = GL_BACK;
		GLenum cull_winding_order = GL_CCW;

		//blending
		bool blend = false;
		GLenum blend_src = GL_ONE;
		GLenum blend_dest = GL_ONE_MINUS_SRC_ALPHA;
		GLenum blend_equation = GL_FUNC_ADD;

		//shadow
		bool cast_shadows = true;
		bool receive_shadows = true;

		template<typename T>
		void set_uniform(const std::string &name, const T& value)
		{
			if constexpr (std::is_same_v<T, bool>) {
				m_uniforms[name].type = shader_type_t::shader_type_bool;
				m_uniforms[name].value = value;
			} else if constexpr (std::is_same_v<T, int>) {
				m_uniforms[name].type = shader_type_t::shader_type_int;
				m_uniforms[name].value = value;
			}
			else if constexpr (std::is_same_v<T, vec2_t>) {
				m_uniforms[name].type = shader_type_t::shader_type_vec2;
				m_uniforms[name].value = value;
			}
			else if constexpr (std::is_same_v<T, vec3_t>) {
				m_uniforms[name].type = shader_type_t::shader_type_vec3;
				m_uniforms[name].value = value;
			}
			else if constexpr (std::is_same_v<T, vec4_t>) {
				m_uniforms[name].type = shader_type_t::shader_type_vec4;
				m_uniforms[name].value = value;
			}
			else if constexpr (std::is_same_v<T, mat4_t>) {
				m_uniforms[name].type = shader_type_t::shader_type_mat4;
				m_uniforms[name].value = value;
			}
			else if constexpr (std::is_same_v<T, mat3_t>) {
				m_uniforms[name].type = shader_type_t::shader_type_mat3;
				m_uniforms[name].value = value;
			}
		}

		void set_texture(const std::string& name, const texture_t& texture, uint32_t unit);
		void set_texture_cube(const std::string& name, const texture_cube_t& texture, uint32_t unit);
	};
}
