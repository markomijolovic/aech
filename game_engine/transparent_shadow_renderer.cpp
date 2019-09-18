#include "transparent_shadow_renderer.hpp"
#include "main.hpp"

#include "mesh_filter.hpp"

#include "transforms.hpp"


void graphics::transparent_shadow_renderer_t::update()
{
	shadow_map->bind();
	glViewport(0, 0, shadow_map->width(), shadow_map->height());
	glDisable(GL_CULL_FACE);

	auto& light_transform  = engine.get_component<transform_t>(dirlight);
	auto  light_projection = math::orthographic(-2250, 2250, -2250, 2000, 0, 2250);

	material->shader()->use();
	for (auto entity : entities)
	{
		auto& transform   = engine.get_component<transform_t>(entity);
		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);

		auto light_view_matrix = math::get_view_matrix(light_transform);

		material->shader()->set_uniform("projection", light_projection);
		material->shader()->set_uniform("view", light_view_matrix);
		material->shader()->set_uniform("model", transform.get_transform_matrix());
		material->set_texture("texture_albedo", mesh_filter.material()->get_texture("texture_albedo"), 0);
		material->set_uniforms();

		mesh_filter.mesh()->draw();
	}
	glEnable(GL_CULL_FACE);
}
