#pragma once

#include "framebuffer.hpp"

#include "framebuffer_library.hpp"

#include "material_library.hpp"

#include "mesh_filter.hpp"

#include "mesh_library.hpp"

#include "system.hpp"


namespace aech::graphics
{
	class directional_light_renderer_t : public ecs::system_t
	{
	public:
		framebuffer_t* render_target = &framebuffers["default"];
		mesh_filter_t  mesh_filter{
			mesh_library::default_meshes["quad"].get(),
			&material_library::default_materials["directional"]
		};

		void update();
	};
} // namespace aech::graphics
#pragma once
