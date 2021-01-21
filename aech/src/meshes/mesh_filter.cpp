#include "mesh_filter.hpp"

aech::graphics::mesh_filter_t::mesh_filter_t(mesh_t *m_mesh, material_t *m_material) noexcept
    : m_mesh{m_mesh}
    , m_material{m_material}
{
}

auto aech::graphics::mesh_filter_t::mesh() const noexcept -> aech::graphics::mesh_t *
{
    return m_mesh;
}

auto aech::graphics::mesh_filter_t::material() const noexcept -> aech::graphics::material_t *
{
    return m_material;
}
