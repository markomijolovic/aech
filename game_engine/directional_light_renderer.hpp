#pragma once

#include "framebuffer.hpp"

#include "framebuffer_library.hpp"

#include "material_library.hpp"

#include "mesh_filter.hpp"

#include "mesh_library.hpp"

#include "system.hpp"
#include "render_cache.hpp"
#include "directional_light.hpp"

namespace aech::graphics
{
	class directional_light_renderer_t : public ecs::system_t
	{
	public:
		directional_light_renderer_t(render_cache_t* render_cache, directional_light_t* directional_light);
		
		void update() const;

		[[nodiscard]] framebuffer_t* render_target() const;
		[[nodiscard]] mesh_filter_t mesh_filter() const;
		
	private:
		framebuffer_t* m_render_target = &framebuffers["default"];
		mesh_filter_t  m_mesh_filter{
			mesh_library::default_meshes["quad"].get(),
			&material_library::default_materials["directional"]
		};
		render_cache_t* m_render_cache{};
		directional_light_t* m_directional_light{};

	};
} // namespace aech::graphics
