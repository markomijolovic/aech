#pragma once
#include "system.hpp"
#include <memory>
#include <unordered_map>
#include "aech_types.hpp"


namespace aech::ecs
{
	class system_manager_t
	{
	public:
		template <typename T>
		std::shared_ptr<T> register_system()
		{
			auto type_name = typeid(T).name();

			auto system = std::make_shared<T>();
			m_systems.insert({type_name, system});

			return system;
		}

		template <typename T>
		void set_signature(signature_t signature)
		{
			auto type_name = typeid(T).name();

			m_signatures.insert({type_name, signature});
		}

		void entity_destroyed(entity_t entity);

		void entity_signature_changed(entity_t entity, signature_t entity_signature);

	private:
		std::unordered_map<const char*, signature_t>               m_signatures{};
		std::unordered_map<const char*, std::shared_ptr<system_t>> m_systems{};
	};
} // namespace aech
