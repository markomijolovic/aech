#pragma once
#include "system.hpp"

namespace aech::physics
{
	class physics_system_t : public ecs::system_t
	{
		constexpr static inline float gravity_constant = -9.81F;
		
		void update(float delta_time);
	};
}