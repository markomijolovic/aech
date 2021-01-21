#pragma once

#include "material.hpp"

#include <unordered_map>

namespace aech::graphics {
namespace material_library {
inline std::unordered_map<std::string, material_t> default_materials{};

// creates default material objects
auto generate_default_materials() noexcept -> void;

// create a material from the given template
[[nodiscard]] auto create_material(const std::string &from) noexcept -> material_t;
} // namespace material_library
} // namespace aech::graphics
