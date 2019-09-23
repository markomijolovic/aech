#pragma once
#include "framebuffer.hpp"

#include "framebuffer_library.hpp"

#include "material_library.hpp"

#include "resource_manager.hpp"
#include "system.hpp"


namespace aech::graphics
{
	class transparent_shadow_renderer_t : public ecs::system_t
	{
	public:
		void set_light_transform(transform_t* t);
		void update();

	private:
		material_t*    m_material   = &material_library::default_materials["transparent_shadow"];
		framebuffer_t* m_shadow_map = &framebuffers["shadow_map"];
		transform_t*   m_light_transform{};
	};
} // namespace aech::graphics
