#pragma once

#include "transform.hpp"

namespace aech::graphics {
// directional light component
class directional_light_t {
public:
    directional_light_t() = default;
    directional_light_t(math::vec3_t colour, float intensity, transform_t *transform);

    [[nodiscard]] auto colour() const -> math::vec3_t;
    [[nodiscard]] auto intensity() const -> float;
    [[nodiscard]] auto transform() const -> transform_t *;

private:
    math::vec3_t m_colour{1.0F, 1.0F, 1.0F};
    float        m_intensity = 1.0F;
    transform_t *m_transform{};
};
} // namespace aech::graphics
