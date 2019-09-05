#pragma once

#include "render_target.hpp"
#include "main.hpp"

namespace aech::graphics
{
	class g_buffer_renderer_t : public ecs::system_t
	{
	public:
		entity_t m_camera{};
		render_target_t g_buffer{ screen_width, screen_height, 4, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point};

		void update();
		void draw_entity(entity_t entity) const;
		void setup_g_buffer() const;
	};
}
