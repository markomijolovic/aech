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
#include "reflection_probe.hpp"
#include "transforms.hpp"


namespace aech::graphics
{
	class gi_renderer_t : public ecs::system_t
	{
	public:
		gi_renderer_t(render_cache_t* render_cache, camera_t* camera);

		void create_preprocessed_environment_map(size_t size);
		void create_irradiance_cubemap(size_t size);
		
		void bake_probes();
		void render_ambient_pass();

		[[nodiscard]] material_t*    specular_material() const;
		[[nodiscard]] material_t*    diffuse_material() const;
		[[nodiscard]] framebuffer_t* render_target() const;

		void add_probe(reflection_probe_t&& probe);
		void add_probe(const reflection_probe_t& probe);
		void add_probe(light_probe_t&& probe);
		void add_probe(const light_probe_t& probe);

	private:
		// used for specular GI
		std::vector<reflection_probe_t> m_reflection_probes{};
		
		// used for diffuse GI
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
		material_t* m_ambient_specular_material{&material_library::default_materials["ambient_specular"]};
		material_t* m_ambient_diffuse_material{ &material_library::default_materials["ambient_diffuse"] };

		mesh_t* m_ndc_cube   = mesh_library::default_meshes["cube"].get();
		mesh_t* m_ndc_quad   = mesh_library::default_meshes["quad"].get();
		mesh_t* m_ndc_sphere = mesh_library::default_meshes["sphere"].get();

		mesh_filter_t m_skybox_mf{
			mesh_library::default_meshes["cube"].get(),
			&material_library::default_materials["skybox"]
		};

		inline static const auto capture_projection = math::perspective(90, 1, 0.1F, 10.0F);

		inline static const std::array capture_views
		{
			look_at({}, math::vec3_t{1, 0, 0}, {0, -1, 0}),
			look_at({}, math::vec3_t{-1, 0, 0}, {0, -1, 0}),
			look_at({}, math::vec3_t{0, 1, 0}, {0, 0, 1}),
			look_at({}, math::vec3_t{0, -1, 0}, {0, 0, -1}),

			look_at({}, math::vec3_t{0, 0, 1}, {0, -1, 0}),
			look_at({}, math::vec3_t{0, 0, -1}, {0, -1, 0})
		};

		void create_radiance_cubemap(math::vec3_t position, size_t probe_index);

		void process_radiance_map(size_t probe_index);
	};
} // namespace aech::graphics
