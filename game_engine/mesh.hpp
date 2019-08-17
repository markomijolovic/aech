#pragma once

#include "types.hpp"
#include "vec3.hpp"
#include <vector>
#include "vec2.hpp"

namespace aech
{
	class mesh_t
	{
	public:
		std::vector<vec3_t> m_positions{};
		std::vector<uint32_t> m_indices{};
		std::vector<vec2_t> m_uvs{};
		std::vector<vec3_t> m_normals{};
		std::vector<vec3_t> m_tangents{};
		std::vector<vec3_t> m_bitangents{};

		uint32_t m_vao{};
		uint32_t m_vbo{};
		uint32_t m_ebo{};
		
		mesh_t() = default;


		void commit(bool interleave = true);
	};
}
