#include "framebuffer_library.hpp"
#include "main.hpp"
#include "resource_manager.hpp"

void aech::graphics::generate_default_framebuffers()
{
    framebuffers["g_buffer"] = {
        window_manager.width(),
        window_manager.height(),
        4,
        true,
        texture_types::sized_internal_format::rgba16f,
        texture_types::format::rgba,
        texture_types::type::floating_point
    };
    framebuffers["default"] = {
        window_manager.width(),
        window_manager.height(),
        1,
        true,
        texture_types::sized_internal_format::rgba16f,
        texture_types::format::rgba,
        texture_types::type::floating_point
    };
    framebuffers["shadow_map"] = {
        8192,
        8192,
        1,
        true,
        texture_types::sized_internal_format::rgba16f,
        texture_types::format::rgba,
        texture_types::type::floating_point
    };

    framebuffers["ssao"] = {
        window_manager.width(),
        window_manager.height(),
        1,
        true,
        texture_types::sized_internal_format::r16f,
        texture_types::format::r,
        texture_types::type::floating_point
    };

    framebuffers["ssao_blurred"] = {
        window_manager.width(),
        window_manager.height(),
        1,
        true,
        texture_types::sized_internal_format::r16f,
        texture_types::format::r,
        texture_types::type::floating_point
    };

    auto shadow_map_texture = framebuffers["shadow_map"].depth_and_stencil();

    shadow_map_texture->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    aech::graphics::texture_t::unbind();

    framebuffers["brdf"] = {
        512,
        512,
        1,
        true,
        texture_types::sized_internal_format::rg16f,
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

    const auto environment_texture = &resource_manager::texture_cubes["environment"];
    *environment_texture = texture_cube_t {
        1024,
        1024,
        texture_types::sized_internal_format::rgb16f,
        texture_types::format::rgb,
        texture_types::type::floating_point
    };

    framebuffer_cubes["hdr_capture"] = { &resource_manager::texture_cubes["environment"], 1024, 1024 };
}
