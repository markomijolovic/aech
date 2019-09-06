#pragma once

#include "render_target.hpp"
#include "main.hpp"
#include "framebuffer_library.hpp"

namespace aech::graphics
{
	class g_buffer_renderer_t : public ecs::system_t
	{
	public:
		entity_t m_camera{};
		render_target_t* g_buffer = &framebuffers["g_buffer"];

		void update();
		void draw_entity(entity_t entity) const;
		void setup_g_buffer() const;
	};
}
