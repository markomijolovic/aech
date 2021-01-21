#include "directional_light.hpp"

aech::graphics::directional_light_t::directional_light_t(transform_t *transform, math::vec3_t colour, float intensity) noexcept
    : m_colour{colour}
    , m_intensity{intensity}
    , m_transform{transform}
{
}

auto aech::graphics::directional_light_t::colour() const noexcept -> math::vec3_t
{
    return m_colour;
}

auto aech::graphics::directional_light_t::intensity() const noexcept -> float
{
    return m_intensity;
}

auto aech::graphics::directional_light_t::transform() const noexcept -> transform_t &
{
    assert(m_transform);
    return *m_transform;
}
