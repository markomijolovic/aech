#include "mesh.hpp"

#include "mesh_filter.hpp"

#include "opaque_shadow_renderer.hpp"

#include "transform.hpp"

#include "transforms.hpp"

#include "glad/glad.h"

#include "main.hpp"

namespace aech::graphics
{
	opaque_shadow_renderer_t::opaque_shadow_renderer_t(render_cache_t* render_cache, directional_light_t* dirlight) :
		m_dirlight{dirlight},
		m_render_cache{render_cache}
	{
	}


	framebuffer_t* opaque_shadow_renderer_t::render_target() const
	{
		return m_render_target;
	}

	void opaque_shadow_renderer_t::update()
	{
		m_render_target->bind();
		m_render_cache->set_viewport(0, 0, m_render_target->width(), m_render_target->height());
		m_render_cache->clear(clear::color_and_depth_buffer_bit);
		m_render_cache->set_cull(true);
		m_render_cache->set_cull_face(cull_face::back);

		//glViewport(0, 0, shadow_map->width(), shadow_map->height());
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		m_render_cache->set_shader(material->shader());

		//material->shader()->use();
		material->set_uniforms();
		for (auto entity : entities)
		{
			auto& transform   = engine.get_component<transform_t>(entity);
			auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
			auto &scene_node = engine.get_component<scene_node_t>(entity);
			auto light_view_matrix = math::get_view_matrix(*m_dirlight->transform());

			material->shader()->set_uniform("projection", renderer.light_projection);
			material->shader()->set_uniform("view", light_view_matrix);
			material->shader()->set_uniform("model", scene_node.get_transform());
			mesh_filter.mesh()->draw();
		}
	}
} // namespace aech::graphics
