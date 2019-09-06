#pragma once
#include "system.hpp"
#include "directional_light_renderer.hpp"
#include "point_light_renderer.hpp"
#include "shadow_renderer.hpp"
#include "opaque_renderer.hpp"
#include "transparent_renderer.hpp"

namespace aech::graphics
{
	class renderer_t : public ecs::system_t
	{
	public:
		entity_t m_camera{};
		std::shared_ptr<opaque_renderer_t> g_buffer_renderer{};
		std::shared_ptr<directional_light_renderer_t> directional_light_renderer{};
		std::shared_ptr<point_light_renderer_t> point_light_renderer{};
		std::shared_ptr<shadow_renderer_t> shadow_renderer{};
		std::shared_ptr<transparent_renderer_t> transparent_renderer{};

		renderer_t();
		void update();
	};
}
