#include "framebuffer_library.hpp"
#include "main.hpp"

void aech::graphics::generate_default_framebuffers()
{
	framebuffers["g_buffer"] = { screen_width, screen_height, 4, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point };
	framebuffers["default"] = { screen_width, screen_height, 1, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point };
	framebuffers["shadow_map"] = { 4096, 4096, 1, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point };
}
