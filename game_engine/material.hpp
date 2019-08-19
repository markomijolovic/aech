#pragma once
#include "shader.hpp"
#include <unordered_map>
#include "shading_types.hpp"

namespace aech
{
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
	};
}
