#pragma once
#include "vec3.hpp"


namespace aech
{
	class render_target_t;

	struct directional_light_t
	{
		vec3_t colour{ 1.0F, 1.0F, 1.0F};
		// controls the brightness of the light
		// colour is multiplied by this value
		float intensity = 1.0F;
		bool cast_shadows = true;

		render_target_t* shadow_map_rt;
	};
}
