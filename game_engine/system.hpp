#pragma once
#include <set>
#include "aech_types.hpp"

namespace aech::ecs
{
	struct system_t
	{
		std::set<entity_t> entities{};
	};
} // namespace aech
