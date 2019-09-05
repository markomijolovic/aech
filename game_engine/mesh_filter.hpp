#pragma once

namespace aech::graphics
{
	class mesh_t;
	struct material_t;

	/**
	 * mesh filter class links up a mesh with a material
	 * for rendering purposes
	 */
	struct mesh_filter_t
	{
		const mesh_t* mesh{};
		const material_t* material{};
	};
}
