#include "quad.hpp"

aech::quad_t::quad_t()
{
	m_positions =
	{
		{ -1.0f,  1.0f, 0.0f, },
		{ -1.0f, -1.0f, 0.0f, },
		{  1.0f,  1.0f, 0.0f, },
		{  1.0f, -1.0f, 0.0f, },
	};

	m_uvs =
	{
		{ 0.0f, 1.0f, },
		{ 0.0f, 0.0f, },
		{ 1.0f, 1.0f, },
		{ 1.0f, 0.0f, },
	};

	commit();
}
