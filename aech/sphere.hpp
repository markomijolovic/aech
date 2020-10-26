#pragma once

#include "mesh.hpp"

namespace aech::graphics {
class sphere_t : public mesh_t {
public:
    sphere_t(std::uint32_t sectors, std::uint32_t stacks);
};
} // namespace aech::graphics
