#pragma once
#include <variant>
#include "vec2.hpp"
#include "vec4.hpp"
#include "vec3.hpp"
#include "mat4.hpp"
#include "mat3.hpp"
#include "texture.hpp"

namespace aech
{

	enum class shader_type_t
	{
		shader_type_bool,
		shader_type_int,
		shader_type_float,
		shader_type_sampler_1d,
		shader_type_sampler_2d,
		shader_type_sampler_3d,
		shader_type_sampler_cube,
		shader_type_vec2,
		shader_type_vec3,
		shader_type_vec4,
		shader_type_mat2,
		shader_type_mat3,
		shader_type_mat4,
	};
	
	using uniform_raw_value_t =
		std::variant<
		bool,
		int,
		float,
		vec2_t,
		vec3_t,
		vec4_t,
		mat3_t,
		mat4_t
	>;

	struct uniform_value_t
	{
		shader_type_t type;
		uniform_raw_value_t value{};
	};

	struct uniform_value_sampler_t
	{
		shader_type_t type;
		uint32_t unit;
		texture_t texture;
	};

	struct vertex_attribute_t
	{
		shader_type_t type;
		std::string name;
		size_t size;
		uint32_t location;
	};
}
