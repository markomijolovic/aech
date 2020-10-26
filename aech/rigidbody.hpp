#pragma once

#include "vec.hpp"

namespace aech::physics {
// Rigidbody component enables objects to act under the control of physics.
// It can receive forces and torque to make objects move in a realistic way.
class rigidbody_t {
public:
    [[nodiscard]] auto mass() const -> float;
    [[nodiscard]] auto gravity() const -> bool;
    [[nodiscard]] auto kinematic() const -> bool;
    [[nodiscard]] auto velocity() const -> math::vec3_t;
    auto               add_velocity(const math::vec3_t &vel) -> void;

private:
    float        m_mass{};
    bool         m_gravity{};
    bool         m_kinematic{};
    math::vec3_t m_velocity{};
};
} // namespace aech::physics
