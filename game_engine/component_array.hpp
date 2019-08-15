#pragma once
#include "entity_manager.hpp"
#include <unordered_map>

namespace aech
{
	class component_array_i
	{
	public:
		virtual      ~component_array_i() = default;
		virtual void entity_destroyed(entity_t entity) = 0;
	};

	template <typename T>
	class component_array_t : public component_array_i
	{
	public:
		void insert_data(entity_t entity, T component);
		void remove_data(entity_t entity);
		T&   get_data(entity_t entity);
		void entity_destroyed(entity_t entity) override;
	private:
		std::array<T, max_entities> m_component_array{};

		/**
		 * maps entity ID to array index
		 */
		std::unordered_map<entity_t, size_t> m_entity_to_index{};

		/**
		 * maps array index to entity ID
		 */
		std::unordered_map<size_t, entity_t> m_index_to_entity{};

		size_t m_size{};
	};
} // namespace aech
