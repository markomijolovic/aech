#pragma once

#include "directional_light.hpp"
#include "framebuffer.hpp"
#include "framebuffer_library.hpp"
#include "material_library.hpp"
#include "mesh_filter.hpp"
#include "mesh_library.hpp"
#include "render_cache.hpp"
#include "system.hpp"

namespace aech::graphics {
class directional_light_renderer_t : public ecs::system_t {
public:
    directional_light_renderer_t(render_cache_t *render_cache, directional_light_t *directional_light);

    auto update() const -> void;

    [[nodiscard]] auto render_target() const -> framebuffer_t *;
    [[nodiscard]] auto mesh_filter() const -> mesh_filter_t;

private:
    framebuffer_t *m_render_target = &framebuffers["default"];
    mesh_filter_t  m_mesh_filter{
        mesh_library::default_meshes["quad"].get(),
        &material_library::default_materials["directional"]};
    render_cache_t *     m_render_cache{};
    directional_light_t *m_directional_light{};
};
} // namespace aech::graphics
