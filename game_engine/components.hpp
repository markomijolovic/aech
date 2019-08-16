#pragma once
#include "vec3.hpp"

namespace aech
{
	struct gravity_t
	{
		vec3_t force;
	};

	struct rigid_body_t
	{
		vec3_t velocity;
		vec3_t acceleration;
	};

	struct transform_t
	{
		vec3_t position;
		vec3_t rotation;
		vec3_t scale;
	};

	struct player_t
	{
	};

	struct renderable_t
	{
		vec3_t colour;
	};

	struct thrust_t
	{
		vec3_t force;
	};
}
