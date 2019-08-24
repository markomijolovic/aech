#include "mesh_library.hpp"
#include "cube.hpp"

void aech::mesh_library::generate_default_meshes()
{
	default_meshes["cube"] = std::make_unique<cube_t>();
}
