#pragma once
#include "vec3.hpp"
#include "mat4.hpp"

namespace aech
{
	struct transform_t
	{
		math::vec3_t position{};
		math::vec3_t rotation{};
		math::vec3_t scale{1.0f, 1.0f, 1.0f};

		[[nodiscard]] math::mat4_t get_transform_matrix() const;
		[[nodiscard]] math::vec3_t get_forward_vector() const;
	};
}
