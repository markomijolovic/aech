#include "transforms.hpp"
#include "main.hpp"
#include "scene_node.hpp"
#include "transform.hpp"
#include "mesh_filter.hpp"
#include "resource_manager.hpp"
#include "g_buffer_renderer.hpp"
#include "camera.hpp"
#include "material_library.hpp"

namespace aech::graphics
{
	//default deferred renderer
	void g_buffer_renderer_t::update()
	{
		setup_g_buffer();
		// draw to g_buffer
		for (auto entity : entities)
		{
			draw_entity(entity);
		}
		g_buffer.unbind();
	}


	void g_buffer_renderer_t::setup_g_buffer() const
	{
		g_buffer.bind();
		glViewport(0, 0, screen_width, screen_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);

		GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
	}

	void g_buffer_renderer_t::draw_entity(entity_t entity) const
	{
		auto& cameraTransform = engine.get_component<transform_t>(m_camera);
		auto& camera = engine.get_component<camera_t>(m_camera);
		auto view = math::get_view_matrix(cameraTransform);
		auto& transform = engine.get_component<transform_t>(entity);
		auto& scene_node = engine.get_component<scene_node_t>(entity);
		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
		auto shader = mesh_filter.material->m_shader;
		auto model = scene_node.get_transform();
		auto& projection = camera.projection;

		shader->use();
		glBindVertexArray(mesh_filter.mesh->m_vao);
		mesh_filter.material->set_uniforms();
		shader->set_uniform("model", model);
		shader->set_uniform("view", view);
		shader->set_uniform("projection", projection);

		glDrawElements(GL_TRIANGLES, mesh_filter.mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
