#include "transparent_shadow_renderer.hpp"

#include "mesh_filter.hpp"

#include "transforms.hpp"

#include "main.hpp"


void aech::graphics::transparent_shadow_renderer_t::set_light_transform(transform_t* t)
{
	m_light_transform = t;
}

void aech::graphics::transparent_shadow_renderer_t::update()
{
	m_shadow_map->bind();
	glViewport(0, 0, m_shadow_map->width(), m_shadow_map->height());
	glDisable(GL_CULL_FACE);

	auto light_view_matrix = math::get_view_matrix(*m_light_transform);

	m_material->shader()->use();
	for (auto entity : entities)
	{
		auto& transform   = engine.get_component<transform_t>(entity);
		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);

		m_material->shader()->set_uniform("projection", 	renderer.light_projection);
		m_material->shader()->set_uniform("view", light_view_matrix);
		m_material->shader()->set_uniform("model", transform.get_transform_matrix());
		m_material->set_texture("texture_albedo", mesh_filter.material()->get_texture("texture_albedo"), 0);
		m_material->set_uniforms();

		mesh_filter.mesh()->draw();
	}
	glEnable(GL_CULL_FACE);
}
