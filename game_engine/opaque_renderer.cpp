#include "transforms.hpp"
#include "camera.hpp"
#include "vector_math.hpp"

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
		m_render_cache->set_blend(false);
		m_render_cache->set_shader(skybox_mf.material()->shader());
		skybox_mf.material()->shader()->set_uniform("view",
		                                            math::get_view_matrix(*m_camera->transform()));
		skybox_mf.material()->shader()->set_uniform("projection", m_camera->projection());
		skybox_mf.material()->set_uniforms();
		m_render_cache->set_cull(false);
		skybox_mf.mesh()->draw();
		m_render_cache->set_cull(true);
	}

	opaque_renderer_t::opaque_renderer_t(render_cache_t* render_cache, camera_t* camera) :
		m_camera{camera},
		m_render_cache{render_cache}
	{
	}

	void opaque_renderer_t::update()
	{
		setup_g_buffer();

		// sort front to back (roughly) to take advantage of early z testing
		std::set<entity_t, decltype(&renderer.sort_front_to_back)> entities_sorted{&renderer.sort_front_to_back};
		for (auto entity : m_entities)
		{
			auto &scene_node = engine.get_component<scene_node_t>(entity);
			if (!m_camera->sees(scene_node)) continue; // view frustum culling
			entities_sorted.insert(entity);
		}

		for (auto entity: entities_sorted)
			draw_entity(entity);
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

		GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(4, attachments);
	}

	void opaque_renderer_t::draw_entity(entity_t entity) const
	{
		auto  view       = math::get_view_matrix(*m_camera->transform());
		auto& scene_node = engine.get_component<scene_node_t>(entity);
		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
		auto shader     = mesh_filter.material()->shader();
		auto projection = m_camera->projection();

		m_render_cache->set_shader(shader);
		mesh_filter.material()->set_uniforms();
		shader->set_uniform("model", scene_node.get_transform());
		shader->set_uniform("view", view);
		shader->set_uniform("projection", projection);

		mesh_filter.mesh()->draw();
	}
} // namespace aech::graphics
