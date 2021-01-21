#pragma once

#include "engine.hpp"
#include "input_manager.hpp"
#include "renderer.hpp"
#include "window_manager.hpp"

namespace aech {
// global engine variable
inline engine_t engine{};

// global input manager variable
inline input_manager_t input_manager{};

namespace graphics {
// global window manager
inline window_manager_t window_manager{};
// global renderer
inline renderer_t renderer{};
} // namespace graphics
} // namespace aech
