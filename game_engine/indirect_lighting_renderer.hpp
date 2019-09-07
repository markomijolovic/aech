#pragma once
#include "system.hpp"

namespace aech::graphics
{
	class indirect_lighting_renderer_t : public ecs::system_t
	{
	public:

		void update();
	};
}
