#include "transforms.hpp"
#include "main.hpp"
#include "scene_node.hpp"
#include "transform.hpp"
#include "mesh_filter.hpp"
#include "resource_manager.hpp"
#include "camera.hpp"
#include "material_library.hpp"
#include "opaque_renderer.hpp"
#include <iostream>

namespace aech::graphics
{
	void opaque_renderer_t::update()
	{
		setup_g_buffer();
		for (auto entity : entities)
		{
			draw_entity(entity);
		}
	}

	void opaque_renderer_t::setup_g_buffer() const
	{
		render_target->bind();
		glViewport(0, 0, screen_width, screen_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);

		GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
	}

	void opaque_renderer_t::draw_entity(entity_t entity) const
	{
		auto view = math::get_view_matrix(engine.get_component<transform_t>(m_camera));
		auto& scene_node = engine.get_component<scene_node_t>(entity);
		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
		auto shader = mesh_filter.material->m_shader;
		auto model = scene_node.get_transform();
		auto& projection = engine.get_component<camera_t>(m_camera).projection;

		mesh_filter.material->set_uniforms();
		shader->set_uniform("model", model);
		shader->set_uniform("view", view);
		shader->set_uniform("projection", projection);

		glBindVertexArray(mesh_filter.mesh->m_vao);
		if (mesh_filter.material->m_texture_cubes.find("skybox") != mesh_filter.material->m_texture_cubes.end())
		{
			glDisable(GL_CULL_FACE);
			glDrawArrays(GL_TRIANGLES, 0, mesh_filter.mesh->m_positions.size());
			glEnable(GL_CULL_FACE);
		}
		else
		{

			glDrawElements(GL_TRIANGLES, mesh_filter.mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}
}
