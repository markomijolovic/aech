#pragma once
#include "mat4.hpp"
#include <cstdint>


namespace aech
{
	struct camera_t
	{
		math::mat4_t projection;

		static math::mat4_t make_perspective_projection(float fov, float near, float far, uint32_t width, uint32_t height);
	};
}
