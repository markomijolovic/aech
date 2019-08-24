#pragma once
#include "types.hpp"
#include <set>

namespace aech
{
	struct system_t
	{
		std::set<entity_t> entities{};
	};
} // namespace aech
