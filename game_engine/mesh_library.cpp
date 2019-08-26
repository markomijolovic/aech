#include "mesh_library.hpp"
#include "cube.hpp"
#include "sphere.hpp"

void aech::mesh_library::generate_default_meshes()
{
	default_meshes["cube"] = std::make_unique<cube_t>();
	default_meshes["sphere"] = std::make_unique<sphere_t>(36, 18, 1.0F);
}
