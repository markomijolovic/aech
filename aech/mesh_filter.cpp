#include "mesh_filter.hpp"

aech::graphics::mesh_filter_t::mesh_filter_t(mesh_t *m_mesh, material_t *m_material)
    : m_mesh{m_mesh}
    , m_material{m_material}
{
}

auto aech::graphics::mesh_filter_t::mesh() const -> aech::graphics::mesh_t *
{
    return m_mesh;
}

auto aech::graphics::mesh_filter_t::material() const -> aech::graphics::material_t *
{
    return m_material;
}
