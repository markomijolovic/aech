#include "cube.hpp"

namespace aech::graphics
{
	using namespace math;

	cube_t::cube_t() :
		mesh_t
		{
			{
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
			},

			{
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
			},

			{

				{-0.5F, -0.5F, -0.5F, 1.0F},
				{0.5F, 0.5F, 0.5F, 1.0F}
			},

			{
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
			}
		}
	{
		commit();
	}
} // namespace aech
