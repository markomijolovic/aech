#pragma once
#include "system.hpp"
#include "material_library.hpp"
#include "render_target.hpp"
#include "resource_manager.hpp"

namespace aech::graphics
{
	class shadow_renderer_t : public ecs::system_t
	{
	public:
		entity_t dirlight{};
		render_target_t shadow_map_rt{ 4096, 4096, 1, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point};
		shader_t* shader = &resource_manager::shaders["shadow"];

		void update();
	};
}
