#pragma once
#include "material.hpp"
#include <unordered_map>

namespace aech
{
	namespace material_library
	{
		inline std::unordered_map<std::string, material_t> default_materials{};

		void generate_default_materials();
		material_t create_material(const std::string& from);
	};
}
