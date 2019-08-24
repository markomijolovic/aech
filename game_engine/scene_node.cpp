#include "transforms.hpp"
#include "main.hpp"
#include "transform.hpp"
#include "scene_node.hpp"

namespace aech
{
	scene_node_t::scene_node_t(transform_t* transform, scene_node_t* parent)
		: m_parent{parent}, m_transform{transform}
	{
	}

	void scene_node_t::set_position(const vec3_t& position) const
	{
		m_transform->position = position;
	}

	void scene_node_t::set_rotation(const vec3_t& rotation) const
	{
		m_transform->rotation = rotation;
	}

	void scene_node_t::set_scale(const vec3_t& scale) const
	{
		m_transform->scale = scale;
	}

	void scene_node_t::set_scale(float scale) const
	{
		m_transform->scale = { scale, scale, scale };
	}

	vec3_t scene_node_t::get_local_position() const
	{
		return m_transform->position;
	}

	vec3_t scene_node_t::get_local_scale() const
	{
		return m_transform->scale;
	}

	vec3_t scene_node_t::get_local_rotation() const
	{
		return m_transform->rotation;
	}

	vec3_t scene_node_t::get_world_position()
	{
		const auto transform_matrix = get_transform();
		auto pos = transform_matrix * vec4_t{ m_transform->position, 1.0f };
		return vec3_t{ pos };
	}

	mat4_t scene_node_t::get_transform() const
	{
		auto transform_matrix = rotate(m_transform->rotation.x, { 1.0f, 0.0f, 0.0f });
		transform_matrix *= rotate(m_transform->rotation.y, { 0.0f, 1.0f, 0.0f });
		transform_matrix *= rotate(m_transform->rotation.z, { 0.0f, 0.0f, 1.0f });

		transform_matrix *= scale(m_transform->scale);
		transform_matrix *= translate(m_transform->position);

		if (m_parent)
		{
			transform_matrix = m_parent->get_transform() * transform_matrix;
		}

		return transform_matrix;
	}

	vec3_t scene_node_t::get_world_scale() const
	{
		auto transform = get_transform();
		return { transform.data[0][0], transform.data[1][1], transform.data[2][2] };
	}

	void scene_node_t::add_child(scene_node_t* node)
	{
		if (node->m_parent)
		{
			node->m_parent->remove_child(node);
		}
		node->m_parent = this;
		m_children.push_back(node);
	}

	void scene_node_t::remove_child(scene_node_t* node)
	{
		
		auto it = std::find(std::begin(m_children), std::end(m_children), node);
		if (it != std::end(m_children))
		{
			m_children.erase(it);
		}
	}
}
