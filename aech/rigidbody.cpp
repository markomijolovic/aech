#include "rigidbody.hpp"

auto aech::physics::rigidbody_t::mass() const -> float
{
    return m_mass;
}

auto aech::physics::rigidbody_t::gravity() const -> bool
{
    return m_gravity;
}

auto aech::physics::rigidbody_t::kinematic() const -> bool
{
    return m_kinematic;
}

auto aech::physics::rigidbody_t::velocity() const -> aech::math::vec3_t
{
    return m_velocity;
}

auto aech::physics::rigidbody_t::add_velocity(const math::vec3_t &vel) -> void
{
    m_velocity += vel;
}
