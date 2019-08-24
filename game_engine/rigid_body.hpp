#pragma once
#include "vec3.hpp"
#include "gravity.hpp"

namespace aech
{
	struct rigid_body_t
	{
		float mass{ 1.0f };
		vec3_t velocity{};
		vec3_t acceleration{ gravity };
	};
}
