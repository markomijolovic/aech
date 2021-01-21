#pragma once

#include "camera.hpp"
#include "directional_light_renderer.hpp"
#include "system.hpp"

namespace aech::graphics {
class point_light_renderer_t : public ecs::system_t {
public:
    point_light_renderer_t(render_cache_t *render_cache, camera_t *camera) noexcept;
    auto update() noexcept -> void;

private:
    camera_t *      m_camera{};
    render_cache_t *m_render_cache{};
};
} // namespace aech::graphics
