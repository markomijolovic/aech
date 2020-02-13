#pragma once
#include "camera.hpp"

#include "framebuffer_library.hpp"

#include "material_library.hpp"

#include "mesh_filter.hpp"

#include "mesh_library.hpp"

#include "render_cache.hpp"
#include "system.hpp"


namespace aech::graphics
{
	class opaque_renderer_t : public ecs::system_t
	{
	public:
		opaque_renderer_t(render_cache_t* render_cache, camera_t* camera);

		void                         update();
		void                         setup_g_buffer() const;
		void                         draw_entity(entity_t entity) const;
		void                         draw_skybox() const;
		[[nodiscard]] framebuffer_t* render_target() const;

	private:
		camera_t*      m_camera{};
		framebuffer_t* m_render_target = &framebuffers["g_buffer"];
		mesh_filter_t  m_skybox_mf     = mesh_filter_t{
			mesh_library::default_meshes["cube"].get(),
			&material_library::default_materials["skybox"]
		};
		render_cache_t* m_render_cache{};
	};
} // namespace aech::graphics
