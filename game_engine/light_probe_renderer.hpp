#pragma once
#include "system.hpp"
#include "light_probe.hpp"
#include <vector>
#include "material.hpp"
#include "resource_manager.hpp"
#include "material_library.hpp"
#include "mesh_library.hpp"

namespace aech::graphics
{
	class light_probe_renderer_t : public ecs::system_t
	{
	public:
		std::vector<light_probe_t> light_probes{};

		material_t* prefilter_material{ &material_library::default_materials["prefilter"] };
		material_t* cubemap_capture_material {&material_library::default_materials["capture"]};
		material_t* irradiance_capture_material{ &material_library::default_materials["irradiance"] };
		mesh_t* ndc_cube = mesh_library::default_meshes["cube"].get();

		void bake_probes();
		void create_radiance_cubemap(size_t probe_index);

		void process_radiance_map(size_t probe_index);
	};
}
