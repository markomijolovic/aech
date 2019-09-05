#pragma once
#include "vec3.hpp"
#include "render_target.hpp"

namespace aech::graphics
{
	struct point_light_t
	{
		math::vec3_t colour { 1.0F, 1.0F, 1.0F };
		float range = 10.0F;
		// controls the brightness of the light
		// colour is multiplied by this value
		float intensity = 1.0F;		
		bool cast_shadows = true;

		render_target_t* shadow_map_rt;
	};
}
