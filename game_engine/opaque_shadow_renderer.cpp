#include "main.hpp"
#include "transform.hpp"
#include "transforms.hpp"
#include "mesh.hpp"
#include "mesh_filter.hpp"
#include <glad/glad.h>
#include "opaque_shadow_renderer.hpp"

namespace aech::graphics
{
	void opaque_shadow_renderer_t::update()
	{
		shadow_map->bind();
		glViewport(0, 0, shadow_map->width, shadow_map->height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);

		auto& light_transform = engine.get_component<transform_t>(dirlight);
		auto light_projection = math::orthographic(-2250, 2250, -2250, 2000, 0, 2250);

		material->m_shader->use();
		material->set_uniforms();
		for (auto entity : entities)
		{
			auto& transform = engine.get_component<transform_t>(entity);
			auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);

			auto light_view_matrix = math::get_view_matrix(light_transform);

			material->m_shader->set_uniform("projection", light_projection);
			material->m_shader->set_uniform("view", light_view_matrix);
			material->m_shader->set_uniform("model", transform.get_transform_matrix());
			mesh_filter.mesh->draw();
		}
		glEnable(GL_CULL_FACE);
	}
}
