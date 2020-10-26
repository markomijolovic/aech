#pragma once

#include "scene_node.hpp"
#include "texture_cube.hpp"

namespace aech::graphics {
class reflection_probe_t {
public:
    reflection_probe_t(math::vec3_t position, scene_node_t *scene_node);
    reflection_probe_t() = default;

    [[nodiscard]] auto prefiltered() const -> texture_cube_t *;
    [[nodiscard]] auto position() const -> math::vec3_t;
    [[nodiscard]] auto scene_node() const -> scene_node_t *;

    auto set_prefiltered(texture_cube_t *prefiltered) -> void;

private:
    texture_cube_t *m_prefiltered{};
    scene_node_t *  m_scene_node{};
    math::vec3_t    m_position{};
};
} // namespace aech::graphics
