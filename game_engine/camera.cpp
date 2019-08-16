#include "camera.hpp"
#include <cmath>
#include "types.hpp"

namespace aech
{
	mat4_t camera_t::make_projection_matrix(float fov, float near_clip, float far_clip, unsigned int view_width, unsigned int view_height)
	{
		auto z_clip_bias0 =
			(far_clip + near_clip)
			/ (far_clip - near_clip);

		auto z_clip_bias1 =
			(2.0f * far_clip * near_clip)
			/ (far_clip - near_clip);

		auto zoom_x = 1.0f / tanf((fov / 2.0f) * (pi/ 180.0f));
		auto zoom_y = (zoom_x * static_cast<float>(view_width)) / static_cast<float>(view_height);

		mat4_t transform{};
		transform.data[0][0] = zoom_x;
		transform.data[1][1] = zoom_y;
		transform.data[2][2] = -z_clip_bias0;
		transform.data[3][2] = -1;
		transform.data[2][3] = -z_clip_bias1;

		return transform;
	}
}
