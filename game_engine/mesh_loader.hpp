#pragma once
#include "mesh.hpp"
#include <vector>
#include "assimp/importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace aech::mesh_loader
{
	inline std::vector<mesh_t*> meshes{};

	void clean();
	SceneNode *
	
}
