#pragma once
#include "mat4.hpp"

#include "scene_node.hpp"
#include "texture_cube.hpp"

#include "vec3.hpp"


namespace aech::graphics
{
	class reflection_probe_t
	{
	public:
		reflection_probe_t(math::vec3_t position, scene_node_t* scene_node);
		reflection_probe_t() = default;

		[[nodiscard]] texture_cube_t* prefiltered() const;
		[[nodiscard]] math::vec3_t    position() const;
		[[nodiscard]] scene_node_t*   scene_node() const;


		void set_prefiltered(texture_cube_t* prefiltered);
	private:
		texture_cube_t* m_prefiltered{};
		scene_node_t*   m_scene_node{};
		math::vec3_t    m_position{};
	};
} // namespace aech::graphics
