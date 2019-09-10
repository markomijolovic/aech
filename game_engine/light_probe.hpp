#pragma once
#include "texture_cube.hpp"
#include "vec3.hpp"

namespace aech::graphics
{
	struct light_probe_t
	{
		texture_cube_t* irradiance{};
		texture_cube_t* prefiltered{};

		math::vec3_t position{};
		float radius{};
	};
}
