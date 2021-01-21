#include "mesh_library.hpp"

#include "cube.hpp"
#include "quad.hpp"
#include "sphere.hpp"

auto aech::graphics::mesh_library::generate_default_meshes() noexcept -> void
{
    default_meshes["sphere"] = std::make_unique<sphere_t>(16, 16);
    default_meshes["cube"]   = std::make_unique<cube_t>();
    default_meshes["quad"]   = std::make_unique<quad_t>();
}
