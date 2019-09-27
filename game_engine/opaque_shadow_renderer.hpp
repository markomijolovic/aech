#pragma once
#include "framebuffer.hpp"

#include "framebuffer_library.hpp"

#include "material_library.hpp"

#include "resource_manager.hpp"

#include "system.hpp"
#include "render_cache.hpp"
#include "directional_light_renderer.hpp"
#include "directional_light.hpp"


namespace aech::graphics
{
	class opaque_shadow_renderer_t : public ecs::system_t
	{
	public:
		opaque_shadow_renderer_t(render_cache_t* render_cache, directional_light_t* dirlight);
		void update();
		[[nodiscard]] framebuffer_t* render_target() const;

	private:
		directional_light_t* m_dirlight{};
		render_cache_t* m_render_cache{};
		framebuffer_t* m_render_target = &framebuffers["shadow_map"];
		material_t*    material   = &material_library::default_materials["opaque_shadow"];
	};
} // namespace aech::graphics
