#pragma once

#include "directional_light_renderer.hpp"
#include "gi_renderer.hpp"
#include "opaque_renderer.hpp"
#include "opaque_shadow_renderer.hpp"
#include "point_light_renderer.hpp"
#include "render_cache.hpp"
#include "transforms.hpp"
#include "transparent_renderer.hpp"
#include "transparent_shadow_renderer.hpp"

namespace aech::graphics {
// main renderer component that bundles together various rendering components
class renderer_t {
public:
    auto init() -> void;
    auto bake_probes() const -> void;
    auto update() -> void;

    [[nodiscard]] auto render_cache() -> render_cache_t *;
    [[nodiscard]] auto ssao() const -> bool;
    [[nodiscard]] auto ssao_texture() const -> const texture_t *;
    [[nodiscard]] auto shadows() const -> bool;
    [[nodiscard]] auto options() const -> bool;
    [[nodiscard]] auto poisson_sampling_distance() const -> float;
    auto               set_options(bool gui) -> void;

    [[nodiscard]] static auto sort_front_to_back(entity_t a, entity_t b) -> bool;
    [[nodiscard]] static auto sort_back_to_front(entity_t a, entity_t b) -> bool;
    inline static auto        light_projection{math::orthographic(-22.50, 22.50, -22.50, 20.00, 0, 22.50)};

private:
    inline static constexpr std::uint32_t ssao_kernel_size{32};

    camera_t *                                     m_camera{};
    std::shared_ptr<point_light_renderer_t>        point_light_renderer{};
    std::shared_ptr<opaque_shadow_renderer_t>      opaque_shadow_renderer{};
    std::shared_ptr<transparent_shadow_renderer_t> transparent_shadow_renderer{};
    std::shared_ptr<transparent_renderer_t>        transparent_renderer{};
    std::shared_ptr<gi_renderer_t>                 gi_renderer{};
    std::shared_ptr<opaque_renderer_t>             m_opaque_renderer{};
    std::unique_ptr<directional_light_renderer_t>  m_directional_light_renderer{};
    std::vector<reflection_probe_t>                m_probes{};
    render_cache_t                                 m_render_cache{};
    mesh_t *                                       m_ndc_cube{};
    mesh_t *                                       m_screen_quad{};
    framebuffer_cube_t *                           m_hdr_capture_fbo{};
    framebuffer_cube_t *                           m_irradiance_fbo{};
    framebuffer_t *                                post_process_fbo{};
    framebuffer_t *                                m_ssao_fbo{};
    framebuffer_t *                                m_ssao_blurred_fbo{};
    framebuffer_t *                                m_gbuffer{};
    shader_t *                                     m_hdr_to_cubemap_shader{};
    shader_t *                                     m_irradiance_shader{};
    shader_t *                                     m_specular_prefilter_shader{};
    shader_t *                                     m_post_process_shader{};
    shader_t *                                     m_tonemap_shader{};

    shader_t *m_ssao_shader{};
    shader_t *m_ssao_blur_shader{};

    std::unique_ptr<texture_t> m_ssao_noise_texture{};
    std::vector<math::vec3_t>  m_ssao_kernel{};

    bool  m_shadows{true};
    float m_poisson_sampling_distance{1.337F};
    float m_ssao_hemisphere_sampling_radius{5.0F};
    bool  m_fxaa{true};
    bool  m_ssao{true};
    bool  m_options{};

    auto render_ssao() -> void;
    auto render_gui() -> void;
    auto post_process() -> void;
};
} // namespace aech::graphics
