#pragma once
#include "system.hpp"
#include "material_library.hpp"
#include "mesh_library.hpp"
#include "mesh_filter.hpp"
#include "framebuffer.hpp"
#include "framebuffer_library.hpp"

namespace aech::graphics
{
	class transparent_renderer_t : public ecs::system_t
	{
	public:
		entity_t m_camera{};
		entity_t dirlight{};

		framebuffer_t* render_target = &framebuffers["default"];
		mesh_filter_t mesh_filter{ mesh_library::default_meshes["quad"].get(), &material_library::default_materials["transparent"] };

		void update();
	};
}
