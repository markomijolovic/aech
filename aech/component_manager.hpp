#pragma once
#include "component_array.hpp"

#include <memory>
#include <unordered_map>
#include <typeindex>


namespace aech::ecs
{
	class component_manager_t
	{
	public:
		template <typename T>
		void register_component()
		{
			// NOTE: cv-qualifiers are ignored by std::type_info
			const auto &type_info = typeid(std::remove_reference_t<T>);
			const auto &type_index = std::type_index{type_info};
	
			m_component_types.insert({type_index, m_next_component_type++});
			m_component_arrays.insert({type_index, std::make_shared<component_array_t<T>>()});
		}

		template <typename T>
		component_type_t get_component_type()
		{
			const auto &type_info = typeid(std::remove_reference_t<T>);
			const auto &type_index = std::type_index{type_info};
			
			return m_component_types[type_index];
		}

		/**
		 * adds a component to the array for the specified entity
		 */
		template <typename T>
		void add_component(entity_t entity, T&& component)
		{
			get_component_array<T>()->insert_data(entity, std::forward<T>(component));
		}

		template <typename T>
		T& get_component(entity_t entity)
		{
			return get_component_array<T>()->get_data(entity);
		}

		template <typename T>
		void remove_component(entity_t entity)
		{
			get_component_array<T>()->remove_data(entity);
		}

		template <typename T>
		bool has_component(entity_t entity)
		{
			return get_component_array<T>()->has_entity(entity);
		}

		void entity_destroyed(entity_t entity);
	private:

		std::unordered_map<std::type_index, component_type_t>                   m_component_types{};
		std::unordered_map<std::type_index, std::shared_ptr<component_array_i>> m_component_arrays{};
		component_type_t                                                    m_next_component_type{};

		/**
		 * helper function to get pointer to the component_array_t of type T
		 */
		template <typename T>
		std::shared_ptr<component_array_t<T>> get_component_array()
		{
			const auto &type_info = typeid(std::remove_reference_t<T>);
			const auto &type_index = std::type_index{type_info};
	

			return std::static_pointer_cast<component_array_t<T>>(m_component_arrays[type_index]);
		}
	};
} // namespace aech
