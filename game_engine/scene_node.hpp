#pragma once
#include "mesh.hpp"
#include "material.hpp"
#include "main.hpp"

namespace aech
{
	/**
	 * a scene node represents a mesh in a scene
	 * and links it to the material that is used to render it
	 * 
	 * currently, a scene node is identified by an entity id
	 * (idk if this is desirable or not)
	 */
	struct scene_node_t
	{
		std::unique_ptr<mesh_t> m_mesh{};
		std::unique_ptr<material_t> m_material{};

		//std::vector<scene_node_t*> m_children{};
		// store entity ids of children
		std::vector<entity_t> m_children{};
		entity_t m_parent = invalid_entity_id; // set to invalid entity id
		//scene_node_t* m_parent;

		mat4_t m_transform{};
		mat4_t m_prev_transform{};

		vec3_t m_position{ 0.0f, 0.0f, 0.0f };
		vec4_t m_rotation{ 0.0f, 0.0f,0.0f,0.0f }; // axis + angle (not a quat)
		vec3_t m_scale{ 1.0f, 1.0f, 1.0f };

		bool m_ditry{};
		entity_t m_id; // entity id this component belongs to (find better solution)

		vec3_t get_local_position();
		vec3_t get_local_scale();
		vec4_t get_local_rotation();
		mat4_t& get_transform();
		vec3_t get_world_position();
		vec3_t get_world_scale();
		void set_position(const vec3_t& position);
		void set_rotation(const vec4_t& rotation);
		void set_scale(const vec3_t& scale);
		void set_scale(float scale);

		explicit scene_node_t(entity_t id);

		void add_child(entity_t node);
		void remove_child(uint32_t id);

		void update_transform(bool update_previous = false);
	};
}
