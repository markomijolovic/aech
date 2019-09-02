#pragma once

#include <glad/glad.h>
#include "types.hpp"
#include "render_target.hpp"
#include "main.hpp"

namespace aech
{
	class g_buffer_renderer_t : public system_t
	{
	public:
		entity_t m_camera{};
		render_target_t g_buffer{ screen_width, screen_height, GL_HALF_FLOAT, 4, true };

		void update();
		void draw_entity(entity_t entity) const;
		void setup_g_buffer() const;
	};
}
