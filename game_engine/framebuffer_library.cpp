#include "framebuffer_library.hpp"
#include "main.hpp"
#include "resource_manager.hpp"

void aech::graphics::generate_default_framebuffers()
{
	framebuffers["g_buffer"] = { screen_width, screen_height, 4, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point };
	framebuffers["default"] = { screen_width, screen_height, 1, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point };
	framebuffers["shadow_map"] = { 4096, 4096, 1, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point };
	framebuffers["brdf_lut"] = { 128, 128, 1, true, texture_types::sized_internal_format::rgba32f, texture_types::format::rgba, texture_types::type::floating_point };

	auto& environment_texture = resource_manager::texture_cubes["environment"];
	environment_texture.width = 1024;
	environment_texture.height = 1024;
	environment_texture.sized_internal_format = texture_types::sized_internal_format::rgb32f;
	environment_texture.format = texture_types::format::rgb;
	environment_texture.type = texture_types::type::floating_point;
	environment_texture.init();
	framebuffer_cubes["hdr_capture"] = {&resource_manager::texture_cubes["environment"], 1024, 1024};
}
