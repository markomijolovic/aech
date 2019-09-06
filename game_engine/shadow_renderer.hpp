#pragma once
#include "system.hpp"
#include "material_library.hpp"
#include "resource_manager.hpp"
#include "framebuffer_library.hpp"
#include "framebuffer.hpp"

namespace aech::graphics
{
	class shadow_renderer_t : public ecs::system_t
	{
	public:
		entity_t dirlight{};

		framebuffer_t* shadow_map = &framebuffers["shadow_map"];
		shader_t* shader = &resource_manager::shaders["shadow"];

		void update();
	};
}
