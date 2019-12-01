#include "quad.hpp"

aech::graphics::quad_t::quad_t()
{
	m_positions =
	{
		{-1.0F, 1.0F, 0.0F,},
		{-1.0F, -1.0F, 0.0F,},
		{1.0F, 1.0F, 0.0F,},
		{1.0F, -1.0F, 0.0F,},
	};

	m_uvs =
	{
		{0.0F, 1.0F,},
		{0.0F, 0.0F,},
		{1.0F, 1.0F,},
		{1.0F, 0.0F,},
	};

	top = topology::triangle_strip;
	commit();
}
