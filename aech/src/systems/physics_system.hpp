#pragma once

#include "system.hpp"

namespace aech::physics {
class physics_system_t : public ecs::system_t {
public:
    constexpr static inline float gravity_constant = -9.81F;

private:
    auto update(float delta_time) noexcept -> void;
};
} // namespace aech::physics
