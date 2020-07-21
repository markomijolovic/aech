#pragma once

#include "vec.hpp"

namespace aech::physics
{
	// Rigidbody component enables objects to act under the control of physics.
	// It can receive forces and torque to make objects move in a realistic way.
	class rigidbody_t
	{
	public:
		[[nodiscard]] float        mass() const;
		[[nodiscard]] bool         gravity() const;
		[[nodiscard]] bool         kinematic() const;
		[[nodiscard]] math::vec3_t velocity() const;
		void                       add_velocity(const math::vec3_t& vel);

	private:
		float        m_mass{};
		bool         m_gravity{};
		bool         m_kinematic{};
		math::vec3_t m_velocity{};
	};
} // namespace aech::physics
