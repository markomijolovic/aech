#pragma once
#include "system.hpp"
#include "light_probe.hpp"
#include <vector>
#include "material.hpp"
#include "resource_manager.hpp"
#include "material_library.hpp"
#include "mesh_library.hpp"
#include "camera.hpp"
#include "framebuffer.hpp"
#include "framebuffer_library.hpp"

namespace aech::graphics
{
	class light_probe_renderer_t : public ecs::system_t
	{
	public:
		std::vector<light_probe_t> light_probes{};
		transform_t* camera_transform{};
		camera_t* camera{};
		framebuffer_t* render_target = &framebuffers["default"];

		material_t* prefilter_material{ &material_library::default_materials["prefilter"] };
		material_t* cubemap_capture_material {&material_library::default_materials["capture"]};
		material_t* cubemap_capture_skybox_material {&material_library::default_materials["capture_skybox"]};
		material_t* irradiance_capture_material{ &material_library::default_materials["irradiance"] };
		material_t* brdf_material{ &material_library::default_materials["brdf"] };
		material_t* ambient_material{ &material_library::default_materials["ambient"] };

		mesh_t* ndc_cube = mesh_library::default_meshes["cube"].get();
		mesh_t* ndc_quad = mesh_library::default_meshes["quad"].get();
		mesh_t* ndc_sphere = mesh_library::default_meshes["sphere"].get();

		void bake_probes();
		void create_radiance_cubemap(size_t probe_index);

		void process_radiance_map(size_t probe_index);
		void render_ambient_pass();
	};
}
