#include "directional_light_renderer.hpp"
#include "directional_light.hpp"

#include "mesh_filter.hpp"

#include "shader.hpp"

#include "transforms.hpp"

#include "main.hpp"

aech::graphics::directional_light_renderer_t::directional_light_renderer_t(render_cache_t*      render_cache,
                                                                           directional_light_t* directional_light) :
	m_render_cache{render_cache},
	m_directional_light{directional_light}
{
}

void aech::graphics::directional_light_renderer_t::update() const
{
	m_render_cache->set_shader(m_mesh_filter.material()->shader());
	m_render_target->bind();
	m_render_cache->set_viewport(0, 0, m_render_target->width(), m_render_target->height());
	m_render_cache->set_cull(false);
	m_render_cache->set_depth_test(false);
	m_render_cache->set_blend(true);
	m_render_cache->set_blend(blend_func::one, blend_func::one);

	auto light_view = math::get_view_matrix(*m_directional_light->transform());
	const static math::mat4_t bias_matrix
	{
		0.5F,
		0,
		0,
		0.5F,
		0,
		0.5F,
		0,
		0.5F,
		0,
		0,
		0.5F,
		0.5F,
		0,
		0,
		0,
		1
	};

	m_mesh_filter.material()->shader()->set_uniform("shadows", renderer.shadows());
	m_mesh_filter.material()->shader()->set_uniform("poisson_sampling_distance_multiplier",
	                                                renderer.poisson_sampling_distance());
	m_mesh_filter.material()->shader()->
	              set_uniform("light_dir", m_directional_light->transform()->get_forward_vector());
	m_mesh_filter.material()->shader()->set_uniform("light_colour", m_directional_light->colour());
	m_mesh_filter.material()->shader()->set_uniform("light_intensity", m_directional_light->intensity());
	m_mesh_filter.material()->shader()->set_uniform("depth_bias_vp", bias_matrix * renderer.light_projection * light_view);
	m_mesh_filter.material()->set_uniforms();

	m_mesh_filter.mesh()->draw();
}

aech::graphics::framebuffer_t* aech::graphics::directional_light_renderer_t::render_target() const
{
	return m_render_target;
}

aech::graphics::mesh_filter_t aech::graphics::directional_light_renderer_t::mesh_filter() const
{
	return m_mesh_filter;
}