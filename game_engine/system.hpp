#pragma once
#include "aech_types.hpp"

#include <set>


namespace aech::ecs
{
	struct system_t
	{
		std::set<entity_t> m_entities{};
	};
} // namespace aech
