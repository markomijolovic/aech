#pragma once

#include "framebuffer.hpp"
#include "framebuffer_cube.hpp"

#include <string>
#include <unordered_map>

namespace aech::graphics {
inline std::unordered_map<std::string, framebuffer_t>      framebuffers{};
inline std::unordered_map<std::string, framebuffer_cube_t> framebuffer_cubes{};

auto generate_default_framebuffers() -> void;
} // namespace aech::graphics
