#include "camera.hpp"
#include "main.hpp"

void aech::camera_t::recalculate_frustum()
{
	auto pv = projection * math::get_view_matrix(*transform);

	math::vec4_t m0
	{
		pv[0][0],
		pv[0][1],
		pv[0][2],
		pv[0][3]
	};
	math::vec4_t m1
	{
		pv[1][0],
		pv[1][1],
		pv[1][2],
		pv[1][3]
	};
	math::vec4_t m2
	{
		pv[2][0],
		pv[2][1],
		pv[2][2],
		pv[2][3]
	};
	math::vec4_t m3
	{
		pv[3][0],
		pv[3][1],
		pv[3][2],
		pv[3][3]
	};

	frustum.planes[0] = (m0 + m3);
	frustum.planes[1] = (m3 - m0);
	frustum.planes[2] = (m3 + m1);
	frustum.planes[3] = (m3 - m1);
	frustum.planes[4] = (m3 + m2);
	frustum.planes[5] = (m3 - m2);
}
