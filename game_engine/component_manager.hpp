#pragma once
#include "component_array.hpp"

#include <memory>
#include <unordered_map>


namespace aech::ecs
{
	class component_manager_t
	{
	public:
		template <typename T>
		void register_component()
		{
			using without_r = std::remove_reference_t<T>;
			using with_lr = std::add_lvalue_reference_t<without_r>;
			using with_rr = std::add_rvalue_reference_t<without_r>;

			auto type_name_without_r = typeid(without_r).name();
			auto type_name_with_lr   = typeid(with_lr).name();
			auto type_name_with_rr   = typeid(with_rr).name();

			m_component_types.insert({type_name_without_r, m_next_component_type++});
			m_component_arrays.insert({type_name_without_r, std::make_shared<component_array_t<T>>()});
			m_component_arrays.insert({type_name_with_lr, m_component_arrays[type_name_without_r]});
			m_component_arrays.insert({type_name_with_rr, m_component_arrays[type_name_without_r]});
		}

		template <typename T>
		component_type_t get_component_type()
		{
			auto type_name = typeid(std::remove_reference_t<T>).name();

			return m_component_types[type_name];
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

		void entity_destroyed(entity_t entity);
	private:

		std::unordered_map<const char*, component_type_t>                   m_component_types{};
		std::unordered_map<const char*, std::shared_ptr<component_array_i>> m_component_arrays{};
		component_type_t                                                    m_next_component_type{};

		/**
		 * helper function to get pointer to the component_array_t of type T
		 */
		template <typename T>
		std::shared_ptr<component_array_t<T>> get_component_array()
		{
			auto type_name = typeid(T).name();

			return std::static_pointer_cast<component_array_t<T>>(m_component_arrays[type_name]);
		}
	};
} // namespace aech
