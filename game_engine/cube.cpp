#include "cube.hpp"

namespace aech
{
	cube_t::cube_t()
	{
		m_positions = {
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(0.5f, -0.5f, -0.5f),
			vec3_t(0.5f, 0.5f, -0.5f),
			vec3_t(0.5f, 0.5f, -0.5f),
			vec3_t(-0.5f, 0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, 0.5),
			vec3_t(0.5f, -0.5f, 0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, -0.5f, 0.5),
			vec3_t(-0.5f, 0.5f, 0.5f),
			vec3_t(-0.5f, 0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, 0.5f),
			vec3_t(-0.5f, 0.5f, 0.5f),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(0.5f, 0.5f, -0.5),
			vec3_t(0.5f, -0.5f, -0.5),
			vec3_t(0.5f, -0.5f, -0.5),
			vec3_t(0.5f, -0.5f, 0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(0.5f, -0.5f, -0.5f),
			vec3_t(0.5f, -0.5f, 0.5f),
			vec3_t(0.5f, -0.5f, 0.5f),
			vec3_t(-0.5f, -0.5f, 0.5f),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(-0.5f, 0.5f, -0.5),
			vec3_t(0.5f, 0.5f, -0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, 0.5f, -0.5)
		};

		// m_uvs = ...

		m_normals = {
		vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0)
		};

		commit();
	}
}
