#pragma once
#include "material.hpp"
#include <unordered_map>

namespace aech::graphics
{
	namespace material_library
	{
		inline std::unordered_map<std::string, material_t> default_materials{};

		void       generate_default_materials();
		material_t create_material(const std::string& from);
	} // namespace material_library
}     // namespace aech::graphics
