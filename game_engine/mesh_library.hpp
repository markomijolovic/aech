#pragma once
#include <unordered_map>
#include <memory>

namespace aech {
	class mesh_t;
}

namespace aech::mesh_library
{
	inline std::unordered_map<std::string, std::unique_ptr<mesh_t>> default_meshes{};

	void generate_default_meshes();
}
