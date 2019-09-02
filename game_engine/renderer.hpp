#pragma once
#include "system.hpp"
#include "g_buffer_renderer.hpp"
#include "directional_light_renderer.hpp"
#include "point_light_renderer.hpp"

namespace aech
{
	class renderer_t : public system_t
	{
	public:
		entity_t m_camera{};

		std::shared_ptr<g_buffer_renderer_t> g_buffer_renderer{};
		std::shared_ptr<directional_light_renderer_t> directional_light_renderer{};
		std::shared_ptr<point_light_renderer_t> point_light_renderer{};

		renderer_t();
		void update();
	};
}
