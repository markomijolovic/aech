#pragma once
#include "framebuffer.hpp"

#include "framebuffer_library.hpp"

#include "material_library.hpp"

#include "directional_light.hpp"

#include "directional_light_renderer.hpp"

#include "resource_manager.hpp"

#include "system.hpp"


namespace aech::graphics
{
	class transparent_shadow_renderer_t : public ecs::system_t
	{
	public:
		transparent_shadow_renderer_t(render_cache_t* render_cache, directional_light_t* directional_light);
		void update();

	private:
		material_t* m_material = &material_library::default_materials["transparent_shadow"];
		framebuffer_t* m_shadow_map = &framebuffers["shadow_map"];
		directional_light_t* m_dirlight{};
		render_cache_t* m_render_cache{};
	};
} // namespace aech::graphics
