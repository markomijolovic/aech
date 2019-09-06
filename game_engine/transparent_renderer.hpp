#pragma once
#include "system.hpp"
#include "material_library.hpp"
#include "mesh_library.hpp"
#include "mesh_filter.hpp"


namespace aech::graphics
{
	class transparent_renderer_t : public ecs::system_t
	{
		mesh_filter_t mesh_filter{ mesh_library::default_meshes["quad"].get(), &material_library::default_materials["opaque"] };

		void update();
	};
}
