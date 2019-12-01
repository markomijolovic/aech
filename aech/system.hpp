#pragma once
#include "aech_types.hpp"

#include <set>


namespace aech::ecs
{
	class system_t
	{
	public:
		std::set<entity_t> m_entities{};
	};
} // namespace aech
