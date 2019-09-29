#pragma once
#include "aech_types.hpp"

#include "system.hpp"

#include <memory>

#include <unordered_map>


namespace aech::ecs
{
	class system_manager_t
	{
	public:
		template <typename T, typename... Args>
		std::shared_ptr<T> register_system(Args&&...args)
		{
			auto type_name = typeid(T).name();

			auto system = std::make_shared<T>(std::forward<Args>(args)...);
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
