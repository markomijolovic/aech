#pragma once
#include "vec3.hpp"
#include "mat4.hpp"


namespace aech
{
	class render_target_t;

	struct directional_light_t
	{
		vec3_t direction{};
		vec3_t colour{ 1.0f, 1.0f, 1.0f };
		float intensity = 1.0f;
		bool cast_shadows = true;

		render_target_t* shadow_map_rt;
		mat4_t light_space_view_projection{};
	};
}
