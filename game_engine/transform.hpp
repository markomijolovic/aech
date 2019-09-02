#pragma once
#include "vec3.hpp"
#include "mat4.hpp"

namespace aech
{
	struct transform_t
	{
		vec3_t position{};
		vec3_t rotation{};
		vec3_t scale{1.0f, 1.0f, 1.0f};

		[[nodiscard]] mat4_t get_transform_matrix() const;
		[[nodiscard]] vec3_t get_forward_vector() const;
	};
}
