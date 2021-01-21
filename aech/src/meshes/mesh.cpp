#include "mesh.hpp"

#include <algorithm>
#include <utility>

namespace aech::graphics {
mesh_t::mesh_t(
    std::vector<math::vec3_t>  positions,
    std::vector<math::vec3_t>  normals,
    std::vector<math::vec2_t>  uvs,
    topology                   top,
    std::vector<std::uint32_t> indices,
    std::vector<math::vec3_t>  tangents,
    std::vector<math::vec3_t>  bitangents) noexcept
    : m_positions{std::move(positions)}
    , m_indices{std::move(indices)}
    , m_uvs{std::move(uvs)}
    , m_normals{std::move(normals)}
    , m_tangents{std::move(tangents)}
    , m_bitangents{std::move(bitangents)}
    , top{top}
{
    commit();
}

auto mesh_t::commit(bool interleave) noexcept -> void
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    std::vector<float> data{};

    if (interleave) {
        for (std::size_t i = 0; i < m_positions.size(); i++) {
            data.push_back(m_positions[i].x);
            data.push_back(m_positions[i].y);
            data.push_back(m_positions[i].z);

            if (!m_uvs.empty()) {
                data.push_back(m_uvs[i].x);
                data.push_back(m_uvs[i].y);
            }

            if (!m_normals.empty()) {
                data.push_back(m_normals[i].x);
                data.push_back(m_normals[i].y);
                data.push_back(m_normals[i].z);
            }

            if (!m_tangents.empty()) {
                data.push_back(m_tangents[i].x);
                data.push_back(m_tangents[i].y);
                data.push_back(m_tangents[i].z);
            }

            if (!m_bitangents.empty()) {
                data.push_back(m_bitangents[i].x);
                data.push_back(m_bitangents[i].y);
                data.push_back(m_bitangents[i].z);
            }
        }
    } else // !interleave
    {
        for (const auto &m_position: m_positions) {
            data.push_back(m_position.x);
            data.push_back(m_position.y);
            data.push_back(m_position.z);
        }

        for (const auto &m_uv: m_uvs) {
            data.push_back(m_uv.x);
            data.push_back(m_uv.y);
        }

        for (const auto &m_normal: m_normals) {
            data.push_back(m_normal.x);
            data.push_back(m_normal.y);
            data.push_back(m_normal.z);
        }

        for (const auto &m_tangent: m_tangents) {
            data.push_back(m_tangent.x);
            data.push_back(m_tangent.y);
            data.push_back(m_tangent.z);
        }

        for (const auto &m_bitangent: m_bitangents) {
            data.push_back(m_bitangent.x);
            data.push_back(m_bitangent.y);
            data.push_back(m_bitangent.z);
        }
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    if (!m_indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     m_indices.size() * sizeof(std::uint32_t),
                     m_indices.data(),
                     GL_STATIC_DRAW);
    }

    if (interleave) {
        auto stride = 3 * sizeof(float);
        if (!m_uvs.empty()) {
            stride += 2 * sizeof(float);
        }
        if (!m_normals.empty()) {
            stride += 3 * sizeof(float);
        }
        if (!m_tangents.empty()) {
            stride += 3 * sizeof(float);
        }
        if (!m_bitangents.empty()) {
            stride += 3 * sizeof(float);
        }

        std::size_t offset{};

        glVertexAttribPointer(0, 3, GL_FLOAT, 0U, static_cast<GLsizei>(stride), reinterpret_cast<GLvoid *>(offset));
        glEnableVertexAttribArray(0);
        offset += 3 * sizeof(float);

        if (!m_uvs.empty()) {
            glVertexAttribPointer(1,
                                  2,
                                  GL_FLOAT,
                                  0U,
                                  static_cast<GLsizei>(stride),
                                  reinterpret_cast<GLvoid *>(offset));
            glEnableVertexAttribArray(1);
            offset += 2 * sizeof(float);
        }

        if (!m_normals.empty()) {
            glVertexAttribPointer(2,
                                  3,
                                  GL_FLOAT,
                                  0U,
                                  static_cast<GLsizei>(stride),
                                  reinterpret_cast<GLvoid *>(offset));
            glEnableVertexAttribArray(2);
            offset += 3 * sizeof(float);
        }

        if (!m_tangents.empty()) {
            glVertexAttribPointer(3,
                                  3,
                                  GL_FLOAT,
                                  0U,
                                  static_cast<GLsizei>(stride),
                                  reinterpret_cast<GLvoid *>(offset));
            glEnableVertexAttribArray(3);
            offset += 3 * sizeof(float);
        }

        if (!m_bitangents.empty()) {
            glVertexAttribPointer(4,
                                  3,
                                  GL_FLOAT,
                                  0U,
                                  static_cast<GLsizei>(stride),
                                  reinterpret_cast<GLvoid *>(offset));
            glEnableVertexAttribArray(4);
        }
    } else // !interleave
    {
        std::size_t offset{};

        glVertexAttribPointer(0, 3, GL_FLOAT, 0U, 0, reinterpret_cast<GLvoid *>(offset));
        glEnableVertexAttribArray(0);
        offset += m_positions.size() * sizeof(float);

        if (!m_uvs.empty()) {
            glVertexAttribPointer(1, 2, GL_FLOAT, 0U, 0, reinterpret_cast<GLvoid *>(offset));
            glEnableVertexAttribArray(1);
            offset += m_uvs.size() * sizeof(float);
        }

        if (!m_normals.empty()) {
            glVertexAttribPointer(2, 3, GL_FLOAT, 0U, 0, reinterpret_cast<GLvoid *>(offset));
            glEnableVertexAttribArray(2);
            offset += m_normals.size() * sizeof(float);
        }

        if (!m_tangents.empty()) {
            glVertexAttribPointer(3, 3, GL_FLOAT, 0U, 0, reinterpret_cast<GLvoid *>(offset));
            glEnableVertexAttribArray(3);
            offset += m_tangents.size() * sizeof(float);
        }

        if (!m_bitangents.empty()) {
            glVertexAttribPointer(4, 3, GL_FLOAT, 0U, 0, reinterpret_cast<GLvoid *>(offset));
            glEnableVertexAttribArray(4);
        }
    }

    glBindVertexArray(0);
}

auto mesh_t::draw() const noexcept -> void
{
    glBindVertexArray(m_vao);
    if (!m_indices.empty()) {
        glDrawElements(static_cast<GLenum>(top), static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(static_cast<GLenum>(top), 0, static_cast<GLsizei>(m_positions.size()));
    }
    glBindVertexArray(0);
}

auto mesh_t::calculate_aabb() const noexcept -> bounding_box_t
{
    bounding_box_t aabb{};

    for (const auto &pos: m_positions) {
        aabb.min_coords.x = std::min(aabb.min_coords.x, pos.x);
        aabb.min_coords.y = std::min(aabb.min_coords.y, pos.y);
        aabb.min_coords.z = std::min(aabb.min_coords.z, pos.z);

        aabb.max_coords.x = std::max(aabb.max_coords.x, pos.x);
        aabb.max_coords.y = std::max(aabb.max_coords.y, pos.y);
        aabb.max_coords.z = std::max(aabb.max_coords.z, pos.z);
    }

    return aabb;
}
} // namespace aech::graphics
