#include "mesh_filter.hpp"

aech::graphics::mesh_filter_t::mesh_filter_t(mesh_t* m_mesh, material_t* m_material)
	: m_mesh{m_mesh}, m_material{m_material}
{
}

aech::graphics::mesh_t* aech::graphics::mesh_filter_t::mesh()
{
	return m_mesh;
}

aech::graphics::material_t* aech::graphics::mesh_filter_t::material()
{
	return m_material;
}
