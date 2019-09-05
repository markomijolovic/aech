#pragma once
#include "vec3.hpp"

namespace aech::graphics
{
	class render_target_t;

	struct directional_light_t
	{
		math::vec3_t colour{ 1.0F, 1.0F, 1.0F};
		// controls the brightness of the light
		// colour is multiplied by this value
		float intensity = 1.0F;
	};
}
