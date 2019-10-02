#pragma once
#include "material.hpp"
#include "mesh.hpp"

#include "transform.hpp"

#include "vec3.hpp"
#include "bounding_box.hpp"


namespace aech::graphics
{
	/**
	 * represents a node in the scene
	 * TODO: optimize transform computation with caching
	 */
	class scene_node_t
	{
	public:
		[[nodiscard]] math::vec3_t get_local_position() const;
		[[nodiscard]] math::vec3_t get_local_scale() const;
		[[nodiscard]] math::vec3_t get_local_rotation() const;
		[[nodiscard]] math::mat4_t get_transform() const;
		[[nodiscard]] math::vec3_t get_world_position() const;
		[[nodiscard]] math::vec3_t get_world_scale() const;
		[[nodiscard]] bounding_box_t bounding_box() const;
		void                       set_position(const math::vec3_t& position);
		void                       set_rotation(const math::vec3_t& rotation);
		void                       set_scale(const math::vec3_t& scale);
		void                       set_scale(float scale);
		void parent_changed() const;

		// make array happy
		scene_node_t() = default;
		scene_node_t(transform_t* transform, scene_node_t* parent = nullptr);

		void add_child(scene_node_t* node);
		void remove_child(scene_node_t* node);

		void set_aabb(const bounding_box_t& aabb);
	
	private:
		std::vector<scene_node_t*> m_children{};
		bounding_box_t m_aabb{};
		scene_node_t*              m_parent{};
		transform_t*               m_transform{};
		// transform cache
		mutable math::mat4_t m_transform_matrix{};
		mutable bool m_dirty{true};
	};
} // namespace aech::graphics
