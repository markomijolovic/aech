#pragma once

#include "system.hpp"

namespace aech
{
	class physics_system_t : public system_t
	{
	public:
		void init();
		void update(float dt);
	};
}