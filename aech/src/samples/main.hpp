#pragma once

#include "engine.hpp"
#include "input_manager.hpp"
#include "renderer.hpp"
#include "window_manager.hpp"

namespace aech {
inline engine_t engine{};
inline input_manager_t input_manager{};

namespace graphics {
inline window_manager_t window_manager{};
inline renderer_t renderer{};
} // namespace graphics
} // namespace aech
