#include "scene_node.hpp"
#include "transforms.hpp"

namespace aech
{
	scene_node_t::scene_node_t(entity_t id)
		: m_id{id}
	{
	}


	void scene_node_t::set_position(const vec3_t& position)
	{
		m_position = position;
		m_ditry = true;
	}

	void scene_node_t::set_rotation(const vec4_t& rotation)
	{
		m_rotation = rotation;
		m_ditry = true;
	}

	void scene_node_t::set_scale(const vec3_t& scale)
	{
		m_scale = scale;
		m_ditry = true;
	}

	void scene_node_t::set_scale(float scale)
	{
		m_scale = {scale, scale, scale};
		m_ditry = true;
	}

	vec3_t scene_node_t::get_local_position()
	{
		return m_position;
	}

	vec3_t scene_node_t::get_local_scale()
	{
		return m_scale;
	}

	vec4_t scene_node_t::get_local_rotation()
	{
		return m_rotation;
	}

	vec3_t scene_node_t::get_world_position()
	{
		auto transform = get_transform();
		auto pos = transform * vec4_t{ m_position, 1.0f };
		return { pos };
	}


	mat4_t& scene_node_t::get_transform()
	{
		if (m_ditry )
		{
			update_transform(false);
		}
		return m_transform;
	}

	vec3_t scene_node_t::get_world_scale()
	{
		auto transform = get_transform();
		return { transform.data[0][0], transform.data[1][1], transform.data[2][2] };
	}

	void scene_node_t::add_child(entity_t node)
	{
		auto &scene_node = engine.get_component<scene_node_t>(node);
		if (scene_node.m_parent != invalid_entity_id)
		{
			auto &parent = engine.get_component<scene_node_t>(scene_node.m_parent);
			parent.remove_child(node);
		}
		scene_node.m_parent = m_id;
		m_children.push_back(node);
	}

	void scene_node_t::remove_child(entity_t id)
	{
		
		auto it = std::find(std::begin(m_children), std::end(m_children), id);
		if (it != std::end(m_children))
		{
			m_children.erase(it);
		}
	}

	void scene_node_t::update_transform(bool update_previous)
	{
		if (update_previous)
		{
			m_prev_transform = m_transform;
		}

		if (m_ditry)
		{
			m_transform = rotate(m_rotation.w, vec3_t{ m_rotation });
			m_transform *= scale(m_scale);
			m_transform *= translate(m_position);

			if(m_parent != invalid_entity_id)
			{
				auto &parent = engine.get_component<scene_node_t>(m_parent);
				m_transform = parent.m_transform * m_transform;
			}
		}

		for (auto child : m_children)
		{
			auto &child_node = engine.get_component<scene_node_t>(child);
			if (m_ditry)
			{
				child_node.m_ditry = true;
			}
			child_node.update_transform(update_previous);
		}
	}
}
