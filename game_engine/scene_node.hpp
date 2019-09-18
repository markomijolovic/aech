#pragma once
#include "material.hpp"
#include "mesh.hpp"

#include "transform.hpp"

#include "vec3.hpp"


namespace aech::graphics
{
	/**
	 * a game object is a base class for entities
	 */
	struct scene_node_t
	{
		std::vector<scene_node_t*> m_children{};
		scene_node_t*              m_parent{};
		transform_t*               m_transform{};

		[[nodiscard]] math::vec3_t get_local_position() const;
		[[nodiscard]] math::vec3_t get_local_scale() const;
		[[nodiscard]] math::vec3_t get_local_rotation() const;
		[[nodiscard]] math::mat4_t get_transform() const;
		[[nodiscard]] math::vec3_t get_world_position() const;
		[[nodiscard]] math::vec3_t get_world_scale() const;
		void                       set_position(const math::vec3_t& position) const;
		void                       set_rotation(const math::vec3_t& rotation) const;
		void                       set_scale(const math::vec3_t& scale) const;
		void                       set_scale(float scale) const;

		scene_node_t() = default;
		explicit scene_node_t(transform_t* transform, scene_node_t* parent = nullptr);

		void add_child(scene_node_t* node);
		void remove_child(scene_node_t* node);
	};
} // namespace aech::graphics
