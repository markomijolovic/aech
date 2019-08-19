#pragma once
#include "vec3.hpp"

namespace aech
{
	struct point_light_t
	{
		vec3_t position{};
		vec3_t colour{ 1.0f, 1.0f, 1.0f };
		float radius = 1.0f;
		float intensity = 1.0f;
		bool visible = true;
		bool render_mesh = false;
	};
}
