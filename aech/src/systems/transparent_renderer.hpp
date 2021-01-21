#pragma once

#include "camera.hpp"
#include "directional_light.hpp"
#include "directional_light_renderer.hpp"
#include "framebuffer.hpp"
#include "framebuffer_library.hpp"
#include "material_library.hpp"
#include "mesh_filter.hpp"
#include "mesh_library.hpp"
#include "system.hpp"

namespace aech::graphics {
class transparent_renderer_t : public ecs::system_t {
public:
    transparent_renderer_t(render_cache_t *render_cache, camera_t *camera, directional_light_t *dirlight) noexcept;

    [[nodiscard]] auto render_target() const noexcept -> framebuffer_t *;
    [[nodiscard]] auto mesh_filter() const noexcept -> mesh_filter_t;

    auto update() const noexcept -> void;

private:
    // NOTE: these are all non-owning pointers!!!
    camera_t *           m_camera{};
    directional_light_t *m_dirlight{};
    render_cache_t *     m_render_cache{};
    framebuffer_t *      m_render_target = &framebuffers["g_buffer"];
    mesh_filter_t        m_mesh_filter{
        mesh_library::default_meshes["quad"].get(),
        &material_library::default_materials["transparent"]};
};
} // namespace aech::graphics
