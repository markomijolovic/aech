#pragma once
#include "resource_manager.hpp"

namespace aech
{
	class point_light_renderer_t : public system_t
	{
	public:
		entity_t m_camera{};

		void update();
	};
}
