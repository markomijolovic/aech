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
	framebuffer_t* opaque_renderer_t::render_target() const
	{
		return m_render_target;
	}

	void opaque_renderer_t::draw_skybox() const
	{
		m_render_cache->set_depth_test(true);
		m_render_cache->set_depth_func(depth_func::lequal);

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		m_render_cache->set_shader(skybox_mf.material()->shader());

		//skybox_mf.material()->shader()->use();
		skybox_mf.material()->shader()->set_uniform("view",
		                                            math::get_view_matrix(*m_camera->transform()));
		skybox_mf.material()->shader()->set_uniform("projection", m_camera->projection());
		skybox_mf.material()->set_uniforms();
		m_render_cache->set_cull(false);

		//glDisable(GL_CULL_FACE);
		skybox_mf.mesh()->draw();
		m_render_cache->set_cull(true);

		//glEnable(GL_CULL_FACE);
	}


	opaque_renderer_t::opaque_renderer_t(render_cache_t* render_cache, camera_t* camera) :
		m_camera{camera},
		m_render_cache{render_cache}
	{
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
		m_render_target->bind();
		m_render_cache->set_viewport(0, 0, m_render_target->width(), m_render_target->height());
		m_render_cache->clear(clear::color_and_depth_buffer_bit);
		m_render_cache->set_cull(true);
		m_render_cache->set_depth_test(true);
		m_render_cache->set_blend(false);
		m_render_cache->set_cull_face(cull_face::back);
		m_render_cache->set_depth_func(depth_func::lequal);
		//glViewport(0, 0, window_manager.width(), window_manager.height());
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);
		//glCullFace(GL_BACK);
		//glDepthFunc(GL_LEQUAL);

		GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(4, attachments);
	}

	void opaque_renderer_t::draw_entity(entity_t entity) const
	{
		auto  view       = math::get_view_matrix(*m_camera->transform());
		auto& scene_node = engine.get_component<scene_node_t>(entity);

		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
		// view frustum culling
		if (!m_camera->sees(*mesh_filter.mesh()))
			return;
		auto shader     = mesh_filter.material()->shader();
		auto model      = scene_node.get_transform();
		auto projection = m_camera->projection();

		m_render_cache->set_shader(shader);
		mesh_filter.material()->set_uniforms();
		shader->set_uniform("model", model);
		shader->set_uniform("view", view);
		shader->set_uniform("projection", projection);

		mesh_filter.mesh()->draw();
	}
} // namespace aech::graphics
