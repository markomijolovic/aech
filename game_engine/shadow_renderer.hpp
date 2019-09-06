#pragma once
#include "system.hpp"
#include "material_library.hpp"
#include "render_target.hpp"
#include "resource_manager.hpp"
#include "framebuffer_library.hpp"

namespace aech::graphics
{
	class shadow_renderer_t : public ecs::system_t
	{
	public:
		entity_t dirlight{};

		render_target_t* shadow_map = &framebuffers["shadow_map"];
		shader_t* shader = &resource_manager::shaders["shadow"];

		void update();
	};
}
