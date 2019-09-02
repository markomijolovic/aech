#pragma once
#include "component_manager.hpp"

#include "system_manager.hpp"

#include <memory>
#include "event_manager.hpp"

namespace aech
{
	class engine_t
	{
	public:
		engine_t();
		[[nodiscard]] entity_t create_entity() const;
		void                   destroy_entity(entity_t entity) const;

		template <typename T>
		void register_component()
		{
			m_component_manager->register_component<T>();
		}

		template <typename T>
		void add_component(entity_t entity, T&& component)
		{
			m_component_manager->add_component<T>(entity, std::forward<T>(component));

			auto signature = m_entity_manager->get_signature(entity);
			signature.set(m_component_manager->get_component_type<T>(), true);

			m_entity_manager->set_signature(entity, signature);
			m_system_manager->entity_signature_changed(entity, signature);
		}

		template <typename T>
		void remove_component(entity_t entity)
		{
			m_component_manager->remove_component<T>(entity);

			auto signature = m_entity_manager->get_signature(entity);
			signature.set(m_component_manager->get_component_type<T>(), false);
			m_entity_manager->set_signature(entity, signature);

			m_system_manager->entity_signature_changed(entity, signature);
		}

		template <typename T>
		T& get_component(entity_t entity)
		{
			return m_component_manager->get_component<T>(entity);
		}

		template <typename T>
		component_type_t get_component_type()
		{
			return m_component_manager->get_component_type<T>();
		}

		template <typename T>
		std::shared_ptr<T> register_system()
		{
			return m_system_manager->register_system<T>();
		}

		template <typename T>
		void set_system_signature(signature_t signature)
		{
			m_system_manager->set_signature<T>(signature);
		}

		void add_event_listener(event_id_t event_id, const std::function<void(event_t&)>& listener);
		void send_event(event_t& event);

	private:
		std::unique_ptr<component_manager_t> m_component_manager;
		std::unique_ptr<entity_manager_t>    m_entity_manager;
		std::unique_ptr<system_manager_t>    m_system_manager;
		std::unique_ptr<event_manager_t>	 m_event_manager;
	};
} // namespace aech
