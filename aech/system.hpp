#pragma once

#include "aech_types.hpp"

#include <set>

namespace aech::ecs {
// a class that holds the entities it is interested in
struct system_t {
    std::set<entity_t> entities{};
};
} // namespace aech::ecs
