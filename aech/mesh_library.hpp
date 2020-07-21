#pragma once

#include "mesh.hpp"
#include <memory>
#include <unordered_map>

namespace aech::graphics::mesh_library {
inline std::unordered_map<std::string, std::unique_ptr<mesh_t>> default_meshes {};

// creates default mesh objects
void generate_default_meshes();
} // namespace aech::graphics::mesh_library
