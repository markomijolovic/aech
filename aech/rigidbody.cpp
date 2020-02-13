#include "rigidbody.hpp"

float aech::physics::rigidbody_t::mass() const
{
	return m_mass;
}

bool aech::physics::rigidbody_t::gravity() const
{
	return m_gravity;
}

bool aech::physics::rigidbody_t::kinematic() const
{
	return m_kinematic;
}


aech::math::vec3_t aech::physics::rigidbody_t::velocity() const
{
	return m_velocity;
}

void aech::physics::rigidbody_t::add_velocity(const math::vec3_t& vel)
{
	m_velocity += vel;
}
