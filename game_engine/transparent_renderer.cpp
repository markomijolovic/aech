#include "transparent_renderer.hpp"
#include "camera.hpp"

#include "directional_light.hpp"


#include "scene_node.hpp"

#include "shader.hpp"

#include "transform.hpp"

#include "transforms.hpp"
#include "main.hpp"


aech::graphics::transparent_renderer_t::transparent_renderer_t(render_cache_t*      render_cache,
                                                               camera_t*            camera,
                                                               directional_light_t* dirlight) :
	m_camera{camera},
	m_dirlight{dirlight},
	m_render_cache{render_cache}
{
}

aech::graphics::framebuffer_t* aech::graphics::transparent_renderer_t::render_target() const
{
	return m_render_target;
}

aech::graphics::mesh_filter_t aech::graphics::transparent_renderer_t::mesh_filter() const
{
	return m_mesh_filter;
}

void aech::graphics::transparent_renderer_t::update()
{
	m_render_target->bind();

	m_render_cache->set_shader(m_mesh_filter.material()->shader());
	//m_mesh_filter.material()->shader()->use();

	m_render_cache->set_depth_test(true);
	m_render_cache->set_blend(true);
	m_render_cache->set_blend(blend_func::src_alpha, blend_func::one_minus_src_alpha);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const static math::mat4_t bias_matrix
	{
		{0.5F, 0.0F, 0.0F, 0.5F},
		{0.0F, 0.5F, 0.0F, 0.5F},
		{0.0F, 0.0F, 0.5F, 0.5F},
		{0.0F, 0.0F, 0.0F, 1.0F}
	};

	auto light_view = math::get_view_matrix(*m_dirlight->transform());

	if (renderer.shadows())
	{
		m_mesh_filter.material()->shader()->set_uniform("poisson_sampling_distance_multiplier",
		                                                renderer.poisson_sampling_distance());
	}

	for (auto entity : entities)
	{
		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
		auto& transform   = engine.get_component<transform_t>(entity);
		auto& scene_node  = engine.get_component<scene_node_t>(entity);

		// view frustum culling
		if (!m_camera->sees(*mesh_filter.mesh()))
			continue;

		mesh_filter.material()->set_uniforms();
		mesh_filter.material()->shader()->set_uniform("model", transform.get_transform_matrix());
		mesh_filter.material()->shader()->set_uniform("view", math::get_view_matrix(*m_camera->transform()));
		mesh_filter.material()->shader()->set_uniform("projection", m_camera->projection());
		mesh_filter.material()->shader()->set_uniform("light_dir", m_dirlight->transform()->get_forward_vector());
		mesh_filter.material()->shader()->set_uniform("light_colour", m_dirlight->colour());
		mesh_filter.material()->shader()->set_uniform("light_intensity", m_dirlight->intensity());
		mesh_filter.material()->shader()->set_uniform("depth_bias_vp",
		                                              bias_matrix * renderer_t::
		                                              light_projection * light_view);
		mesh_filter.material()->shader()->set_uniform("camera_position", m_camera->transform()->position);
		mesh_filter.mesh()->draw();
	}
}
