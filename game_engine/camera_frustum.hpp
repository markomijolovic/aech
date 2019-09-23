#pragma once
#include <array>
#include "vec4.hpp"

namespace aech
{
	namespace graphics {
		struct bounding_box_t;
	}

	struct camera_frustum_t
	{
		// planes is an array of 6 frustum planes
		// one plane consists of coefficients a, b, c and d
		// in the plane equation a*x + b*y + c*z + d = 0
		std::array<math::vec4_t, 6> planes{};

		bool intersects(const graphics::bounding_box_t& aabb);
	};
}
