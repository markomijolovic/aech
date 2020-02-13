#pragma once
#include "mat4.hpp"

#include "scene_node.hpp"
#include "texture_cube.hpp"

#include "vec3.hpp"


namespace aech::graphics
{
	class light_probe_t
	{
	public:
		light_probe_t(math::vec3_t position, float inner_radius, float outer_radius, scene_node_t* scene_node);
		light_probe_t() = default;

		// TODO(Marko): store irradiance as spherical harmonics
		[[nodiscard]] texture_cube_t* irradiance() const;
		//[[nodiscard]] texture_cube_t* prefiltered() const;
		[[nodiscard]] math::vec3_t  position() const;
		[[nodiscard]] float         inner_radius() const;
		[[nodiscard]] float         outer_radius() const;
		[[nodiscard]] scene_node_t* scene_node() const;

		void set_irradiance(texture_cube_t* irradiance);
		//void set_prefiltered(texture_cube_t* prefiltered);
	private:
		texture_cube_t* m_irradiance{};
		//texture_cube_t* m_prefiltered{};
		scene_node_t* m_scene_node{};
		math::vec3_t  m_position{};
		float         m_inner_radius{};
		float         m_outer_radius{};
	};
} // namespace aech::graphics
