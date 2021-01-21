#pragma once

#include "scene_node.hpp"
#include "texture_cube.hpp"

namespace aech::graphics {
class reflection_probe_t {
public:
    reflection_probe_t(math::vec3_t position, scene_node_t *scene_node) noexcept;
    reflection_probe_t() = default;

    [[nodiscard]] auto prefiltered() const noexcept -> texture_cube_t *;
    [[nodiscard]] auto position() const noexcept -> math::vec3_t;
    [[nodiscard]] auto scene_node() const noexcept -> scene_node_t *;

    auto set_prefiltered(texture_cube_t *prefiltered) noexcept -> void;

private:
    texture_cube_t *m_prefiltered{};
    scene_node_t *  m_scene_node{};
    math::vec3_t    m_position{};
};
} // namespace aech::graphics
