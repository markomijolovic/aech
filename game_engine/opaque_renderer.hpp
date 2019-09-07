#pragma once

#include "main.hpp"
#include "framebuffer_library.hpp"
#include "framebuffer.hpp"

namespace aech::graphics
{
	class opaque_renderer_t : public ecs::system_t
	{
	public:
		entity_t m_camera{};
		framebuffer_t* render_target = &framebuffers["g_buffer"];

		void update();
		void draw_entity(entity_t entity) const;
		void setup_g_buffer() const;
	};
}
