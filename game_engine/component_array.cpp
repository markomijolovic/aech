#include "component_array.hpp"

namespace aech
{
	template <typename T>
	void component_array_t<T>::insert_data(entity_t entity, T component)
	{
		auto index                = m_size;
		m_entity_to_index[entity] = index;
		m_index_to_entity[index]  = entity;
		m_component_array[index]  = component;
		m_size++;
	}

	template <typename T>
	void component_array_t<T>::remove_data(entity_t entity)
	{
		auto index               = m_entity_to_index[entity];
		auto index_of_last       = m_size - 1;
		m_component_array[index] = m_component_array[index_of_last];

		auto entity_of_last               = m_index_to_entity[index_of_last];
		m_entity_to_index[entity_of_last] = index;
		m_index_to_entity[index]          = entity_of_last;

		m_entity_to_index.erase(entity);
		m_index_to_entity.erase(index_of_last);
		m_size--;
	}

	template <typename T>
	T& component_array_t<T>::get_data(entity_t entity)
	{
		auto index = m_entity_to_index[entity];
		return m_component_array[index];
	}

	template <typename T>
	void component_array_t<T>::entity_destroyed(entity_t entity)
	{
		auto it = m_entity_to_index.find(entity);
		if (it != std::end(m_entity_to_index))
		{
			remove_data(entity);
		}
	}
} // namespace aech
