#pragma once

namespace aech::graphics {
class mesh_t;
class material_t;

// mesh filter class links up a mesh with a material
// for rendering purposes
class mesh_filter_t {
public:
    // make array happy
    mesh_filter_t() = default;
    mesh_filter_t(mesh_t *m_mesh, material_t *m_material);
    [[nodiscard]] auto mesh() const -> mesh_t *;
    [[nodiscard]] auto material() const -> material_t *;

private:
    mesh_t *    m_mesh{};
    material_t *m_material{};
};
} // namespace aech::graphics
