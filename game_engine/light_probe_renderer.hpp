#pragma once
#include "camera.hpp"

#include "framebuffer.hpp"

#include "framebuffer_library.hpp"

#include "light_probe.hpp"

#include "material.hpp"

#include "material_library.hpp"
#include "mesh_filter.hpp"

#include "mesh_library.hpp"

#include "resource_manager.hpp"

#include "system.hpp"

#include <vector>
#include "render_cache.hpp"
#include "directional_light_renderer.hpp"


namespace aech::graphics
{
	class light_probe_renderer_t : public ecs::system_t
	{
	public:
		light_probe_renderer_t(render_cache_t* render_cache, camera_t* camera);

		void bake_probes();
		void render_ambient_pass();

		[[nodiscard]] material_t*    ambient_material() const;
		[[nodiscard]] framebuffer_t* render_target() const;

		void add_probe(light_probe_t&& probe);
		void add_probe(const light_probe_t& probe);

	private:
		std::vector<light_probe_t> m_light_probes{};
		camera_t*                  m_camera{};
		render_cache_t*            m_render_cache{};
		framebuffer_t*             m_render_target = &framebuffers["default"];
		material_t*                m_prefilter_material{&material_library::default_materials["prefilter"]};
		material_t*                m_cubemap_capture_material{&material_library::default_materials["capture"]};
		material_t*                m_cubemap_capture_skybox_material{
			&material_library::default_materials["capture_skybox"]
		};
		material_t* m_cubemap_capture_transparent_material { &material_library::default_materials["capture_transparent"]};
		material_t* m_irradiance_capture_material{&material_library::default_materials["irradiance"]};
		material_t* m_brdf_material{&material_library::default_materials["brdf"]};
		material_t* m_ambient_material{&material_library::default_materials["ambient"]};

		mesh_t* m_ndc_cube   = mesh_library::default_meshes["cube"].get();
		mesh_t* m_ndc_quad   = mesh_library::default_meshes["quad"].get();
		mesh_t* m_ndc_sphere = mesh_library::default_meshes["sphere"].get();

		mesh_filter_t m_skybox_mf{
			mesh_library::default_meshes["cube"].get(),
			&material_library::default_materials["skybox"]
		};

		void create_radiance_cubemap(size_t probe_index);

		void process_radiance_map(size_t probe_index);
	};
} // namespace aech::graphics
