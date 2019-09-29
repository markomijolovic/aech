#include "mat4.hpp"

#include "scene_node.hpp"
#include "transform.hpp"
#include "main.hpp"


namespace aech::graphics
{
	scene_node_t::scene_node_t(transform_t* transform, scene_node_t* parent) :
		m_parent{parent},
		m_transform{transform}
	{
	}

	void scene_node_t::set_position(const math::vec3_t& position) const
	{
		m_transform->position = position;
	}

	void scene_node_t::set_rotation(const math::vec3_t& rotation) const
	{
		m_transform->rotation = rotation;
	}

	void scene_node_t::set_scale(const math::vec3_t& scale) const
	{
		m_transform->scale = scale;
	}

	void scene_node_t::set_scale(float scale) const
	{
		m_transform->scale = {scale, scale, scale};
	}

	math::vec3_t scene_node_t::get_local_position() const
	{
		return m_transform->position;
	}

	math::vec3_t scene_node_t::get_local_scale() const
	{
		return m_transform->scale;
	}

	math::vec3_t scene_node_t::get_local_rotation() const
	{
		return m_transform->rotation;
	}

	math::vec3_t scene_node_t::get_world_position() const
	{
		const auto transform_matrix = get_transform();
		auto       pos              = transform_matrix * math::vec4_t{m_transform->position, 1.0F};
		return math::vec3_t{pos};
	}

	math::mat4_t scene_node_t::get_transform() const
	{
		auto transform_matrix = m_transform->get_transform_matrix();

		if (m_parent != nullptr)
		{
			transform_matrix = m_parent->get_transform() * transform_matrix;
		}

		return transform_matrix;
	}

	math::vec3_t scene_node_t::get_world_scale() const
	{
		auto transform = get_transform();
		return {transform[0][0], transform[1][1], transform[2][2]};
	}

	void scene_node_t::add_child(scene_node_t* node)
	{
		if (node->m_parent != nullptr)
		{
			node->m_parent->remove_child(node);
		}
		node->m_parent = this;
		m_children.push_back(node);
	}

	void scene_node_t::remove_child(scene_node_t* node)
	{
		const auto it = std::find(std::begin(m_children), std::end(m_children), node);
		if (it != std::end(m_children))
		{
			m_children.erase(it);
		}
	}
} // namespace aech::graphics
