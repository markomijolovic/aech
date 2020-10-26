#pragma once

#include "bounding_box.hpp"
#include "vec.hpp"

#include <glad/glad.h>
#include <vector>

namespace aech::graphics {
// a class that represents a mesh
class mesh_t {
public:
    enum class topology {
        triangles      = GL_TRIANGLES,
        triangle_strip = GL_TRIANGLE_STRIP
    };

    // make unordered_map happy
    mesh_t() = default;
    mesh_t(
        std::vector<math::vec3_t> positions,
        std::vector<math::vec3_t> normals,
        std::vector<math::vec2_t> uvs        = {},
        topology                  top        = topology::triangles,
        std::vector<std::uint32_t>     indices    = {},
        std::vector<math::vec3_t> tangents   = {},
        std::vector<math::vec3_t> bitangents = {});
    auto               draw() const -> void;
    [[nodiscard]] auto calculate_aabb() const -> bounding_box_t;

protected:
    auto commit(bool interleave = true) -> void;

    std::vector<math::vec3_t> m_positions{};
    std::vector<std::uint32_t>     m_indices{};
    std::vector<math::vec2_t> m_uvs{};
    std::vector<math::vec3_t> m_normals{};
    std::vector<math::vec3_t> m_tangents{};
    std::vector<math::vec3_t> m_bitangents{};
    std::uint32_t                  m_vao{};
    std::uint32_t                  m_vbo{};
    std::uint32_t                  m_ebo{};
    topology                  top{topology::triangles};
};
} // namespace aech::graphics
