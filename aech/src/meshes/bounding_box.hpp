#pragma once

#include "vec.hpp"

#include <limits>

namespace aech::graphics {
// use std::min and std::max
// to set min and max coords
struct bounding_box_t {
    math::vec4_t min_coords{
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        1.0F};

    math::vec4_t max_coords{
        -std::numeric_limits<float>::max(),
        -std::numeric_limits<float>::max(),
        -std::numeric_limits<float>::max(),
        1.0F};
};
} // namespace aech::graphics
