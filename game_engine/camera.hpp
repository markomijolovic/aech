#pragma once
#include "mat4.hpp"
#include "camera_frustum.hpp"
#include "transform.hpp"

namespace aech
{
	struct camera_t
	{
		math::mat4_t     projection{};
		camera_frustum_t frustum{};
		transform_t*     transform{};

		void recalculate_frustum();
	};
} // namespace aech
