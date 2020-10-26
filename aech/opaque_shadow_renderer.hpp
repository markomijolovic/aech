#pragma once

#include "directional_light.hpp"
#include "framebuffer.hpp"
#include "framebuffer_library.hpp"
#include "material_library.hpp"
#include "render_cache.hpp"
#include "resource_manager.hpp"
#include "system.hpp"

namespace aech::graphics {
class opaque_shadow_renderer_t : public ecs::system_t {
public:
    opaque_shadow_renderer_t(render_cache_t *render_cache, directional_light_t *dirlight);
    auto               update() -> void;
    [[nodiscard]] auto render_target() const -> framebuffer_t *;

private:
    directional_light_t *m_dirlight{};
    render_cache_t *     m_render_cache{};
    framebuffer_t *      m_render_target = &framebuffers["shadow_map"];
    material_t *         m_material      = &material_library::default_materials["opaque_shadow"];
};
} // namespace aech::graphics
