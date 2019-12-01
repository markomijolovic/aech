#pragma once
#include "aech_types.hpp"

#include <any>
#include <unordered_map>


namespace aech::events
{
	class event_t
	{
	public:
		explicit                 event_t(event_id_t type);
		[[nodiscard]] event_id_t type() const;

		template <typename T>
		void set_param(param_id_t id, T&& value)
		{
			m_data[id] = std::forward<T>(value);
		}

		template <typename T>
		T get_param(param_id_t id)
		{
			return std::any_cast<T>(m_data[id]);
		}

	private:
		event_id_t                               m_type{};
		std::unordered_map<param_id_t, std::any> m_data{};
	};
} // namespace aech::events
