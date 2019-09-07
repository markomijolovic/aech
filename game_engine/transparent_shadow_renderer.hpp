#pragma once
#include "system.hpp"
#include "framebuffer.hpp"
#include "framebuffer_library.hpp"
#include "resource_manager.hpp"
#include "material_library.hpp"


namespace aech::graphics
{
	class transparent_shadow_renderer_t : public ecs::system_t
	{
	public:
		entity_t dirlight{};

		framebuffer_t* shadow_map = &framebuffers["shadow_map"];
		material_t* material = &material_library::default_materials["transparent_shadow"];

		void update();
	};
}
