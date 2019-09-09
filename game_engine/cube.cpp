#include "cube.hpp"
using namespace aech::math;

namespace aech::graphics
{
	cube_t::cube_t()
	{
		m_uvs = {
			{0.0F, 0.0F},
			{1.0F, 0.0F},
			{1.0F, 1.0F},
			{1.0F, 1.0F},
			{0.0F, 1.0F},
			{0.0F, 0.0F},
			{0.0F, 0.0F},
			{1.0F, 0.0F},
			{1.0F, 1.0F},
			{1.0F, 1.0F},
			{0.0F, 1.0F},
			{0.0F, 0.0F},
			{0.0F, 0.0F},
			{1.0F, 0.0F},
			{1.0F, 1.0F},
			{1.0F, 1.0F},
			{0.0F, 1.0F},
			{0.0F, 0.0F},
			{0.0F, 0.0F},
			{1.0F, 0.0F},
			{1.0F, 1.0F},
			{1.0F, 1.0F},
			{0.0F, 1.0F},
			{0.0F, 0.0F},
			{0.0F, 0.0F},
			{1.0F, 0.0F},
			{1.0F, 1.0F},
			{1.0F, 1.0F},
			{0.0F, 1.0F},
			{0.0F, 0.0F},
			{0.0F, 0.0F},
			{1.0F, 0.0F},
			{1.0F, 1.0F},
			{1.0F, 1.0F},
			{0.0F, 1.0F},
			{0.0F, 0.0F}
		};

		m_positions = {
			vec3_t(-0.5F, -0.5F, -0.5F),
			vec3_t(0.5F, -0.5F, -0.5F),
			vec3_t(0.5F, 0.5F, -0.5F),
			vec3_t(0.5F, 0.5F, -0.5F),
			vec3_t(-0.5F, 0.5F, -0.5F),
			vec3_t(-0.5F, -0.5F, -0.5F),
			vec3_t(-0.5F, -0.5F, 0.5),
			vec3_t(0.5F, -0.5F, 0.5),
			vec3_t(0.5F, 0.5F, 0.5),
			vec3_t(0.5F, 0.5F, 0.5),
			vec3_t(-0.5F, 0.5F, 0.5),
			vec3_t(-0.5F, -0.5F, 0.5),
			vec3_t(-0.5F, 0.5F, 0.5F),
			vec3_t(-0.5F, 0.5F, -0.5F),
			vec3_t(-0.5F, -0.5F, -0.5F),
			vec3_t(-0.5F, -0.5F, -0.5F),
			vec3_t(-0.5F, -0.5F, 0.5F),
			vec3_t(-0.5F, 0.5F, 0.5F),
			vec3_t(0.5F, 0.5F, 0.5),
			vec3_t(0.5F, 0.5F, -0.5),
			vec3_t(0.5F, -0.5F, -0.5),
			vec3_t(0.5F, -0.5F, -0.5),
			vec3_t(0.5F, -0.5F, 0.5),
			vec3_t(0.5F, 0.5F, 0.5),
			vec3_t(-0.5F, -0.5F, -0.5F),
			vec3_t(0.5F, -0.5F, -0.5F),
			vec3_t(0.5F, -0.5F, 0.5F),
			vec3_t(0.5F, -0.5F, 0.5F),
			vec3_t(-0.5F, -0.5F, 0.5F),
			vec3_t(-0.5F, -0.5F, -0.5F),
			vec3_t(-0.5F, 0.5F, -0.5),
			vec3_t(0.5F, 0.5F, -0.5),
			vec3_t(0.5F, 0.5F, 0.5),
			vec3_t(0.5F, 0.5F, 0.5),
			vec3_t(-0.5F, 0.5F, 0.5),
			vec3_t(-0.5F, 0.5F, -0.5)
		};

		// TODO: LOL FIX THIS
		for (auto& el : m_positions) el *= 2;

		m_normals = {
			vec3_t(0.0F, 0.0F, -1.0F),
			vec3_t(0.0F, 0.0F, -1.0F),
			vec3_t(0.0F, 0.0F, -1.0F),
			vec3_t(0.0F, 0.0F, -1.0F),
			vec3_t(0.0F, 0.0F, -1.0F),
			vec3_t(0.0F, 0.0F, -1.0F),
			vec3_t(0.0F, 0.0F, 1.0F),
			vec3_t(0.0F, 0.0F, 1.0F),
			vec3_t(0.0F, 0.0F, 1.0F),
			vec3_t(0.0F, 0.0F, 1.0F),
			vec3_t(0.0F, 0.0F, 1.0F),
			vec3_t(0.0F, 0.0F, 1.0F),
			vec3_t(-1.0F, 0.0F, 0.0F),
			vec3_t(-1.0F, 0.0F, 0.0F),
			vec3_t(-1.0F, 0.0F, 0.0F),
			vec3_t(-1.0F, 0.0F, 0.0F),
			vec3_t(-1.0F, 0.0F, 0.0F),
			vec3_t(-1.0F, 0.0F, 0.0F),
			vec3_t(1.0F, 0.0F, 0.0F),
			vec3_t(1.0F, 0.0F, 0.0F),
			vec3_t(1.0F, 0.0F, 0.0F),
			vec3_t(1.0F, 0.0F, 0.0F),
			vec3_t(1.0F, 0.0F, 0.0F),
			vec3_t(1.0F, 0.0F, 0.0F),
			vec3_t(0.0F, -1.0F, 0.0F),
			vec3_t(0.0F, -1.0F, 0.0F),
			vec3_t(0.0F, -1.0F, 0.0F),
			vec3_t(0.0F, -1.0F, 0.0F),
			vec3_t(0.0F, -1.0F, 0.0F),
			vec3_t(0.0F, -1.0F, 0.0F),
			vec3_t(0.0F, 1.0F, 0.0F),
			vec3_t(0.0F, 1.0F, 0.0F),
			vec3_t(0.0F, 1.0F, 0.0F),
			vec3_t(0.0F, 1.0F, 0.0F),
			vec3_t(0.0F, 1.0F, 0.0F),
			vec3_t(0.0F, 1.0F, 0.0)
		};

		commit(true);
	}
} // namespace aech
