#pragma once

#include "camera.hpp"
#include "framebuffer.hpp"
#include "framebuffer_library.hpp"
#include "light_probe.hpp"
#include "material.hpp"
#include "material_library.hpp"
#include "mesh_filter.hpp"
#include "mesh_library.hpp"
#include "reflection_probe.hpp"
#include "render_cache.hpp"
#include "system.hpp"
#include "transforms.hpp"

#include <vector>

namespace aech::graphics {
class gi_renderer_t : public ecs::system_t {
public:
    gi_renderer_t(render_cache_t *render_cache, camera_t *camera) noexcept;

    auto create_preprocessed_environment_map(std::size_t probe_index, std::uint32_t width = 128, std::uint32_t height = 128) noexcept -> void;
    auto create_irradiance_cubemap(std::size_t probe_index, std::uint32_t width = 32, std::uint32_t height = 32) noexcept -> void;

    auto bake_probes() noexcept -> void;
    auto render_ambient_pass() noexcept -> void;

    [[nodiscard]] auto specular_material() const noexcept -> material_t *;
    [[nodiscard]] auto diffuse_material() const noexcept -> material_t *;
    [[nodiscard]] auto render_target() const noexcept -> framebuffer_t *;

    auto add_probe(reflection_probe_t &&probe) noexcept -> void;
    auto add_probe(const reflection_probe_t &probe) noexcept -> void;
    auto add_probe(light_probe_t &&probe) noexcept -> void;
    auto add_probe(const light_probe_t &probe) noexcept -> void;

    bool m_diffuse_gi{true};
    bool m_specular_gi{true};
    bool m_dot_product_weight{true};

private:
    // used for specular GI
    std::vector<reflection_probe_t> m_reflection_probes{};

    // used for diffuse GI
    std::vector<light_probe_t> m_light_probes{};

    // NOTE: these are all NON-OWNING pointers!!!
    camera_t *      m_camera{};
    render_cache_t *m_render_cache{};
    framebuffer_t * m_render_target = &framebuffers["default"];
    material_t *    m_prefilter_material{&material_library::default_materials["prefilter"]};
    material_t *    m_cubemap_capture_material{&material_library::default_materials["capture"]};
    material_t *    m_cubemap_capture_skybox_material{&material_library::default_materials["capture_skybox"]};
    material_t *    m_cubemap_capture_transparent_material{&material_library::default_materials["capture_transparent"]};
    material_t *    m_irradiance_capture_material{&material_library::default_materials["irradiance"]};
    material_t *    m_brdf_material{&material_library::default_materials["brdf"]};
    material_t *    m_ambient_specular_material{&material_library::default_materials["ambient_specular"]};
    material_t *    m_ambient_diffuse_material{&material_library::default_materials["ambient_diffuse"]};

    mesh_t *m_ndc_cube   = mesh_library::default_meshes["cube"].get();
    mesh_t *m_ndc_quad   = mesh_library::default_meshes["quad"].get();
    mesh_t *m_ndc_sphere = mesh_library::default_meshes["sphere"].get();

    mesh_filter_t m_skybox_mf{mesh_library::default_meshes["cube"].get(), &material_library::default_materials["skybox"]};

    inline static const auto capture_projection = math::perspective(90, 1, 0.1F, 10.0F);

    inline static const std::array capture_views{look_at({}, math::vec3_t{1, 0, 0}, {0, -1, 0}), look_at({}, math::vec3_t{-1, 0, 0}, {0, -1, 0}), look_at({}, math::vec3_t{0, 1, 0}, {0, 0, 1}), look_at({}, math::vec3_t{0, -1, 0}, {0, 0, -1}), look_at({}, math::vec3_t{0, 0, 1}, {0, -1, 0}), look_at({}, math::vec3_t{0, 0, -1}, {0, -1, 0})};
    auto                           create_radiance_cubemap(math::vec3_t position, std::size_t probe_index, std::uint32_t width = 1024, std::uint32_t height = 1024) noexcept -> void;

    //void process_radiance_map(std::size_t probe_index);
};
} // namespace aech::graphics
