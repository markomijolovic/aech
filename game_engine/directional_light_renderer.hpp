#pragma once

#include "resource_manager.hpp"
#include "material_library.hpp"
#include "mesh_filter.hpp"
#include "mesh_library.hpp"

namespace aech
{
	class directional_light_renderer_t : public system_t
	{
	public:
		mesh_filter_t mesh_filter{ mesh_library::default_meshes["quad"].get(), &material_library::default_materials["directional"] };

		void update();
	};
}
#pragma once
