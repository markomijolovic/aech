#pragma once

#include "vec.hpp"

#include "bounding_box.hpp"

#include <glad/glad.h>

#include <vector>

namespace aech::graphics
{
	// a class that represents a mesh 
	class mesh_t
	{
	public:
		enum class topology
		{
			triangles = GL_TRIANGLES,
			triangle_strip = GL_TRIANGLE_STRIP
		};

		// make unordered_map happy
		mesh_t() = default;
		mesh_t(
			std::vector<math::vec3_t> positions,
			std::vector<math::vec3_t> normals,
			std::vector<math::vec2_t> uvs = {},
			topology top = topology::triangles,
			std::vector<uint32_t> indices = {},
			std::vector<math::vec3_t> tangents = {},
			std::vector<math::vec3_t> bitangents = {}
		);
		void draw() const;
		[[nodiscard]] bounding_box_t calculate_aabb() const;

	protected:
		void commit(bool interleave = true);

		std::vector<math::vec3_t> m_positions{};
		std::vector<uint32_t> m_indices{};
		std::vector<math::vec2_t> m_uvs{};
		std::vector<math::vec3_t> m_normals{};
		std::vector<math::vec3_t> m_tangents{};
		std::vector<math::vec3_t> m_bitangents{};
		uint32_t m_vao{};
		uint32_t m_vbo{};
		uint32_t m_ebo{};
		topology top{topology::triangles};
	};
} // namespace aech::graphics
