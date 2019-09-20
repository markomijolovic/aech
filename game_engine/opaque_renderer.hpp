#pragma once
#include "framebuffer_library.hpp"
#include "mesh_library.hpp"
#include "mesh_filter.hpp"
#include "material_library.hpp"
#include "system.hpp"

namespace aech::graphics
{
	class opaque_renderer_t : public ecs::system_t
	{
	public:
		entity_t       m_camera{};
		framebuffer_t* render_target = &framebuffers["g_buffer"];
		mesh_filter_t  skybox_mf = mesh_filter_t{mesh_library::default_meshes["cube"].get(), &material_library::default_materials["skybox"]};

		void update();
		void setup_g_buffer() const;
		void draw_entity(entity_t entity) const;
		void draw_skybox() const;
	};
} // namespace aech::graphics
