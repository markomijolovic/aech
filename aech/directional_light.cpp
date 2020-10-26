#include "directional_light.hpp"

#include "transform.hpp"

aech::graphics::directional_light_t::directional_light_t(math::vec3_t colour, float intensity, transform_t *transform)
    : m_colour{colour}
    , m_intensity{intensity}
    , m_transform{transform}
{
}

auto aech::graphics::directional_light_t::colour() const -> aech::math::vec3_t
{
    return m_colour;
}

auto aech::graphics::directional_light_t::intensity() const -> float
{
    return m_intensity;
}

auto aech::graphics::directional_light_t::transform() const -> aech::transform_t *
{
    return m_transform;
}
