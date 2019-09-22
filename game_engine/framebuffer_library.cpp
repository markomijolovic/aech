#include "framebuffer_library.hpp"
#include "resource_manager.hpp"
#include "main.hpp"

void aech::graphics::generate_default_framebuffers()
{
	framebuffers["g_buffer"] = {
		window_manager.width(),
		window_manager.height(),
		4,
		true,
		texture_types::sized_internal_format::rgba32f,
		texture_types::format::rgba,
		texture_types::type::floating_point
	};
	framebuffers["default"] = {
		window_manager.width(),
		window_manager.height(),
		1,
		true,
		texture_types::sized_internal_format::rgba32f,
		texture_types::format::rgba,
		texture_types::type::floating_point
	};
	framebuffers["shadow_map"] = {
		4096,
		4096,
		1,
		true,
		texture_types::sized_internal_format::rgba32f,
		texture_types::format::rgba,
		texture_types::type::floating_point
	};

	auto shadow_map_texture = framebuffers["shadow_map"].depth_and_stencil();
	
	shadow_map_texture->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	shadow_map_texture->unbind();
	
	framebuffers["brdf"] = {
		512,
		512,
		1,
		true,
		texture_types::sized_internal_format::rg32f,
		texture_types::format::rg,
		texture_types::type::floating_point
	};

	framebuffers["post_process"] = {
		window_manager.width(),
		window_manager.height(),
		1,
		true,
		texture_types::sized_internal_format::rgba8,
		texture_types::format::rgba,
		texture_types::type::ubyte
	};

	auto environment_texture                 = &resource_manager::texture_cubes["environment"];
	*environment_texture = texture_cube_t{1024, 1024, texture_types::sized_internal_format::rgb32f, texture_types::format::rgb, texture_types::type::floating_point};

	framebuffer_cubes["hdr_capture"] = {&resource_manager::texture_cubes["environment"], 1024, 1024};
}
