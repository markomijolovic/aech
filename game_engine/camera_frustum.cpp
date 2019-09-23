#include "camera_frustum.hpp"
#include "bounding_box.hpp"

bool aech::camera_frustum_t::intersects(const graphics::bounding_box_t& aabb)
{
	// get all vertices of bounding box
	std::array vertices
	{
		aabb.min_coords,
		math::vec4_t{aabb.max_coords.x, aabb.min_coords.y, aabb.min_coords.z, 1.0F},
		math::vec4_t{aabb.min_coords.x, aabb.max_coords.y, aabb.min_coords.z, 1.0F},
		math::vec4_t{aabb.max_coords.x, aabb.max_coords.y, aabb.min_coords.z, 1.0F},
		math::vec4_t{aabb.min_coords.x, aabb.min_coords.y, aabb.max_coords.z, 1.0F},
		math::vec4_t{aabb.max_coords.x, aabb.min_coords.y, aabb.max_coords.z, 1.0F},
		math::vec4_t{aabb.min_coords.x, aabb.max_coords.y, aabb.max_coords.z, 1.0F},
		aabb.max_coords
	};

	// test all vertices against all planes
	for (auto &plane: planes)
	{
		int32_t count{};
		for (auto &vertex: vertices)
		{
			if (dot(vertex, plane) > 0) count++;
		}
		if (!count) return false;
	}

	return true;
}
