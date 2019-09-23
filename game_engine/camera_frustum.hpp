#pragma once
#include <array>
#include "vec4.hpp"
#include "bounding_box.hpp"
#include "mat4.hpp"

namespace aech
{
	// See T. Akenine-Möller, E. Haines, N. Hoffman, A. Pesce, M. Iwanicki, S. Hillaire -
	// Real-Time Rendering-CRC Press (2018) - 22.14 View Frustum Intersection
	struct camera_frustum_t
	{
		// aabb
		[[nodiscard]] bool intersects(const graphics::bounding_box_t& aabb) const;
		// sphere
		[[nodiscard]] bool intersects(const math::vec3_t& centre, float radius) const;
		void recalculate(const math::mat4_t& pv);
		
	private:
		// 6 frustum planes
		// one plane consists of coefficients a, b, c and d
		// in the plane equation a*x + b*y + c*z + d = 0
		std::array<math::vec4_t, 6> planes{};

	};
}
