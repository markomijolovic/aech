#pragma once
#include "types.hpp"
#include <unordered_map>
#include <any>

namespace aech
{
	class event_t
	{
	public:
		explicit  event_t(event_id_t type);
		event_id_t get_type() const;

		template<typename T>
		void set_param(event_id_t id, T value)
		{
			m_data[id] = value;
		}

		template <typename T>
		T get_param(event_id_t id)
		{
			return std::any_cast<T>(m_data[id]);
		}


	private:
		event_id_t m_type{};
		std::unordered_map<event_id_t, std::any> m_data{};
	};
}
