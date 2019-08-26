#pragma once
#include "mesh.hpp"

namespace aech
{
	class sphere_t : public mesh_t
	{
	public:
		sphere_t(uint32_t sectors, uint32_t stacks, float radius);
	};
} // namespace aech
