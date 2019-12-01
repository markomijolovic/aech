#include "rigidbody.hpp"

float aech::physics::rigidbody_t::mass()
{
	return m_mass;
}

bool aech::physics::rigidbody_t::gravity()
{
	return m_gravity;
}

bool aech::physics::rigidbody_t::kinematic()
{
	return m_kinematic;
}


aech::math::vec3_t aech::physics::rigidbody_t::velocity()
{
	return m_velocity;
}

void aech::physics::rigidbody_t::add_velocity(const math::vec3_t& vel)
{
	m_velocity += vel;
}
