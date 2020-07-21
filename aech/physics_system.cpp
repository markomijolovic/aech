#include "physics_system.hpp"
#include "main.hpp"
#include "rigidbody.hpp"

void aech::physics::physics_system_t::update(float /*delta_time*/)
{
    for (auto entity : entities) {
        auto& rigidbody = engine.get_component<rigidbody_t>(entity);

        if (rigidbody.gravity()) {
        }
    }
}
