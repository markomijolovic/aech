#pragma once
#include "framebuffer.hpp"

#include "framebuffer_library.hpp"

#include "material_library.hpp"

#include "mesh_filter.hpp"
#include "mesh_library.hpp"

#include "system.hpp"
#include "transform.hpp"
#include "camera.hpp"
#include "directional_light.hpp"


namespace aech::graphics
{
	class transparent_renderer_t : public ecs::system_t
	{
	public:
		void set_camera_transform(transform_t* t);
		void set_dirlight_transform(transform_t* t);
		void set_camera(camera_t* camera);
		void set_dirlight(directional_light_t* dirlight);

		[[nodiscard]] framebuffer_t* render_target() const;
		[[nodiscard]] mesh_filter_t  mesh_filter() const;

		void update();

	private:
		transform_t*         m_camera_transform{};
		transform_t*         m_dirlight_transform{};
		camera_t*            m_camera{};
		directional_light_t* m_dirlight{};

		framebuffer_t* m_render_target = &framebuffers["default"];
		mesh_filter_t  m_mesh_filter{
			mesh_library::default_meshes["quad"].get(),
			&material_library::default_materials["transparent"]
		};
	};
} // namespace aech::graphics
