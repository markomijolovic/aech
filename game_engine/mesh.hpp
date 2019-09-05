#pragma once

#include "vec3.hpp"
#include <vector>
#include "vec2.hpp"

namespace aech::graphics
{
	class mesh_t
	{
	public:
		std::vector<math::vec3_t> m_positions{};
		std::vector<uint32_t> m_indices{};
		std::vector<math::vec2_t> m_uvs{};
		std::vector<math::vec3_t> m_normals{};
		std::vector<math::vec3_t> m_tangents{};
		std::vector<math::vec3_t> m_bitangents{};

		uint32_t m_vao{};
		uint32_t m_vbo{};
		uint32_t m_ebo{};
		
		mesh_t() = default;
	
		void commit(bool interleave = true);
	};
}
