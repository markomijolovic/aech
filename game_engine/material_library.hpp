#pragma once
#include "material.hpp"
#include <unordered_map>

namespace aech
{
	class material_library_t
	{
	public:
		material_library_t();
		std::unordered_map<std::string, material_t> m_default_materials{};
		std::vector<material_t> m_materials{};

		void generate_default_materials();
	};
}
