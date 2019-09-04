#pragma once
#include "system.hpp"
#include "material_library.hpp"
#include "render_target.hpp"
#include "resource_manager.hpp"

namespace aech
{
	class shadow_renderer_t : public system_t
	{
	public:
		entity_t dirlight{};
		render_target_t shadow_map_rt{ 4096, 4096, GL_FLOAT, 1, true, GL_RGBA32F, GL_RGBA};
		shader_t* shader = &resource_manager::shaders["shadow"];

		void update();
	};
}
