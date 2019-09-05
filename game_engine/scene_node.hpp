#pragma once
#include "mesh.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "transform.hpp"

namespace aech::graphics
{

	/**
	 * a game object is a base class for entities
	 */
	struct scene_node_t
	{
		std::vector<scene_node_t*> m_children{};
		scene_node_t* m_parent{};
		transform_t* m_transform{};

		math::vec3_t get_local_position() const;
		math::vec3_t get_local_scale() const;
		math::vec3_t get_local_rotation() const;
		math::mat4_t get_transform() const;
		math::vec3_t get_world_position() const;
		math::vec3_t get_world_scale() const;
		void set_position(const math::vec3_t& position) const;
		void set_rotation(const math::vec3_t& rotation) const;
		void set_scale(const math::vec3_t& scale) const;
		void set_scale(float scale) const;

		scene_node_t() = default;
		explicit scene_node_t(transform_t* transform, scene_node_t* parent = nullptr);

		void add_child(scene_node_t* node);
		void remove_child(scene_node_t* node);
	};
}
