#pragma once

#include "system.hpp"

namespace aech
{
	class physics_system_t : public system_t
	{
	public:
		void update(float dt);
		physics_system_t() = default;
	};
}