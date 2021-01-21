#pragma once

#include "aech_types.hpp"
#include "transform.hpp"

namespace aech::graphics {
// directional light component
class directional_light_t {
public:
    directional_light_t() = default;
    directional_light_t(transform_t *transform, math::vec3_t colour, float intensity) noexcept;

    [[nodiscard]] auto colour() const noexcept -> math::vec3_t;
    [[nodiscard]] auto intensity() const noexcept -> float;
    [[nodiscard]] auto transform() const noexcept -> transform_t &;

private:
    // NOTE: this is a NON-OWNING pointer!!!
    // it can not be a reference because we require components to be default constructible and we can not determine the proper object to refer to at construction time
    transform_t *m_transform{};
    math::vec3_t m_colour{1.0F, 1.0F, 1.0F};
    float        m_intensity = 1.0F;
};
} // namespace aech::graphics
