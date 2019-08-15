#pragma once
#include "types.hpp"
#include <set>

namespace aech
{
	class system_t
	{
	public:
		std::set<entity_t> m_entities{};
	};
} // namespace aech
