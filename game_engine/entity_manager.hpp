#pragma once
#include <array>

#include <queue>
#include "aech_types.hpp"


namespace aech::ecs
{
	/**
	 * manages individual entity IDs
	 */
	class entity_manager_t
	{
	public:
		entity_manager_t();
		entity_t    create_entity();
		void        destroy_entity(entity_t entity);
		void        set_signature(entity_t entity, signature_t signature);
		signature_t get_signature(entity_t entity);

	private:
		std::queue<entity_t> m_available_entities{};

		/**
		 * an array of signatures
		 * index is an entity id
		 * value is that entity's signature - 
		 * i.e. what components it has
		 */
		std::array<signature_t, max_entities + 1> m_signatures{};

		uint32_t m_living_entities_count{};
	};
} // namespace aech
