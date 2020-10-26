#pragma once

#include "mat.hpp"
#include "scene_node.hpp"
#include "texture_cube.hpp"

namespace aech::graphics {
class light_probe_t {
public:
    light_probe_t(math::vec3_t position, float inner_radius, float outer_radius, scene_node_t *scene_node);
    light_probe_t() = default;

    // TODO(Marko): store irradiance as spherical harmonics
    [[nodiscard]] auto irradiance() const -> texture_cube_t *;
    //[[nodiscard]] texture_cube_t* prefiltered() const;
    [[nodiscard]] auto position() const -> math::vec3_t;
    [[nodiscard]] auto inner_radius() const -> float;
    [[nodiscard]] auto outer_radius() const -> float;
    [[nodiscard]] auto scene_node() const -> scene_node_t *;

    void set_irradiance(texture_cube_t *irradiance);
    //void set_prefiltered(texture_cube_t* prefiltered);
private:
    texture_cube_t *m_irradiance{};
    //texture_cube_t* m_prefiltered{};
    scene_node_t *m_scene_node{};
    math::vec3_t  m_position{};
    float         m_inner_radius{};
    float         m_outer_radius{};
};
} // namespace aech::graphics
