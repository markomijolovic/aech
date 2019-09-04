#include "shadow_renderer.hpp"
#include "main.hpp"
#include "transform.hpp"
#include "transforms.hpp"
#include "mesh.hpp"
#include "mesh_filter.hpp"

namespace aech
{
	void shadow_renderer_t::update()
	{
		auto& light_transform = engine.get_component<transform_t>(dirlight);
		auto light_projection = orthographic(-2500, 2500, -2500, 2500, -10, 2500);

		glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_rt.m_id);
		glViewport(0, 0, shadow_map_rt.m_width, shadow_map_rt.m_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);

		shader->use();
		for (auto entity : entities)
		{
			auto& transform = engine.get_component<transform_t>(entity);
			auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);

			auto light_view_matrix = get_view_matrix(light_transform);

			shader->set_uniform("projection", light_projection);
			shader->set_uniform("view", light_view_matrix);
			shader->set_uniform("model", transform.get_transform_matrix());

			glBindVertexArray(mesh_filter.mesh->m_vao);
			glDrawElements(GL_TRIANGLES, mesh_filter.mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
