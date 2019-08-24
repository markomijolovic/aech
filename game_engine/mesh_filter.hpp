#pragma once

namespace aech
{
	class mesh_t;
	struct material_t;

	/**
	 * mesh filter class links up a mesh with a material
	 * for rendering purposes
	 */
	struct mesh_filter_t
	{
		mesh_t* mesh{};
		material_t* material{};
	};
}
