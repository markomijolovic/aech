#pragma once
#include <vector>
#include "light_probe.hpp"
#include "material_library.hpp"
#include "mesh.hpp"
#include "mesh_library.hpp"
#include "mesh_filter.hpp"


namespace aech::graphics
{
	class ibl_precompute_t
	{
		
		std::vector<light_probe_t> light_probes{};
		light_probe_t sky_probe{};

		material_t* hdr_to_cubemap = &material_library::default_materials["hdr_to_cubemap"];
		material_t* irradiance = &material_library::default_materials["irradiance"];
		material_t* prefilter = &material_library::default_materials["prefilter"];
		material_t* brdf = &material_library::default_materials["brdf"];
		material_t* probe_capture = &material_library::default_materials["capture"];
		material_t* probe_capture_background = &material_library::default_materials["background"];

		mesh_filter_t scene_cube{ mesh_library::default_meshes["cube"].get(), hdr_to_cubemap };

		light_probe_t* process_equirectangular(texture_t* environment);
	};
}
