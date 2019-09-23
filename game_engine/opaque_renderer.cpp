#include "transforms.hpp"
#include "camera.hpp"


#include "material_library.hpp"

#include "mesh_filter.hpp"
#include "opaque_renderer.hpp"

#include "resource_manager.hpp"

#include "scene_node.hpp"

#include "transform.hpp"
#include "main.hpp"

namespace aech::graphics
{
	void opaque_renderer_t::draw_skybox() const
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		skybox_mf.material()->shader()->use();
		skybox_mf.material()->shader()->set_uniform("view",
		                                            math::get_view_matrix(engine.get_component<transform_t>(m_camera)));
		skybox_mf.material()->shader()->set_uniform("projection", engine.get_component<camera_t>(m_camera).projection());
		skybox_mf.material()->set_uniforms();
		glDisable(GL_CULL_FACE);
		skybox_mf.mesh()->draw();
		glEnable(GL_CULL_FACE);
	}

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
		glViewport(0, 0, window_manager.width(), window_manager.height());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);

		GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(4, attachments);
	}

	void opaque_renderer_t::draw_entity(entity_t entity) const
	{
		auto  view        = math::get_view_matrix(engine.get_component<transform_t>(m_camera));
		auto& scene_node  = engine.get_component<scene_node_t>(entity);

		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
		auto &camera = engine.get_component<camera_t>(m_camera);
		// view frustum culling
		if (!camera.sees(*mesh_filter.mesh())) return;
		auto  shader      = mesh_filter.material()->shader();
		auto  model       = scene_node.get_transform();
		auto projection  = camera.projection();

		mesh_filter.material()->set_uniforms();
		shader->set_uniform("model", model);
		shader->set_uniform("view", view);
		shader->set_uniform("projection", projection);

		mesh_filter.mesh()->draw();
	}
} // namespace aech::graphics
