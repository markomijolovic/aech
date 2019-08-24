#pragma once

namespace aech
{
	class mesh_t;
	struct material_t;

	struct mesh_filter_t
	{
		mesh_t* mesh{};
		material_t* material{};
	};
}
