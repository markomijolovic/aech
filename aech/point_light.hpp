#pragma once
#include "vec3.hpp"

namespace aech::graphics
{
	// point light component
	struct point_light_t
	{
		math::vec3_t colour{1.0F, 1.0F, 1.0F};
		float        range = 10.0F;
		// controls the brightness of the light
		// colour is multiplied by this value
		float intensity    = 1.0F;
		bool  cast_shadows = true;

		//framebuffer_t* shadow_map_rt;
	};
} // namespace aech::graphics
