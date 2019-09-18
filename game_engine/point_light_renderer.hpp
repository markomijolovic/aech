#pragma once
#include "system.hpp"

namespace aech::graphics
{
	class point_light_renderer_t : public ecs::system_t
	{
	public:
		entity_t m_camera{};

		void update();
	};
} // namespace aech::graphics
