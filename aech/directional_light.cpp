#include "directional_light.hpp"
#include "transform.hpp"

aech::graphics::directional_light_t::directional_light_t(math::vec3_t colour, float intensity, transform_t* transform)
    : m_colour { colour }
    , m_intensity { intensity }
    , m_transform { transform }
{
}

aech::math::vec3_t aech::graphics::directional_light_t::colour() const
{
    return m_colour;
}

float aech::graphics::directional_light_t::intensity() const
{
    return m_intensity;
}

aech::transform_t* aech::graphics::directional_light_t::transform() const
{
    return m_transform;
}
