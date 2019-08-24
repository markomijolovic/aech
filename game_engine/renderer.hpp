#pragma once

#include <GL/glew.h>
#include "types.hpp"
#include "vec2.hpp"
#include <vector>
#include "directional_light.hpp"
#include "point_light.hpp"
#include "render_target.hpp"
#include "camera.hpp"
#include "material_library.hpp"
#include "system.hpp"

namespace aech
{
	/**
	 * render state needed to render with opengl
	 */
	struct render_state_t
	{
		bool m_depth_test;
		bool m_blend;
		bool m_cull;
		GLenum m_depth_func;
		GLenum m_blend_src;
		GLenum m_blend_dest;
		GLenum m_front_face;
		GLenum m_primitive;
		uint32_t m_shader_id;
	};

	class renderer_t : public system_t
	{
	public:
		bool m_shadows = true;
		bool m_lights = true;
		bool m_render_lights = true;
		entity_t m_camera{};
		vec2_t m_render_size{};
		render_state_t m_render_state{};

		
		std::vector<directional_light_t> m_directional_lights{};
		std::vector<point_light_t> m_point_lights{};
		render_target_t m_render_target{ 0, 0 };

		renderer_t();
		void update(float delta_time);
	};
}
