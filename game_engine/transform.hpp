#pragma once
#include "vec3.hpp"

namespace aech
{
	struct transform_t
	{
		vec3_t position{};
		vec3_t rotation{};
		vec3_t scale{1.0f, 1.0f, 1.0f};
	};
}
