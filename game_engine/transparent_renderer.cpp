#include "transparent_renderer.hpp"
#include "camera.hpp"

#include "directional_light.hpp"


#include "scene_node.hpp"

#include "shader.hpp"

#include "transform.hpp"

#include "transforms.hpp"
#include "main.hpp"


void aech::graphics::transparent_renderer_t::set_camera_transform(transform_t* t)
{
	m_camera_transform = t;
}

void aech::graphics::transparent_renderer_t::set_dirlight_transform(transform_t* t)
{
	m_dirlight_transform = t;
}

void aech::graphics::transparent_renderer_t::set_camera(camera_t* camera)
{
	m_camera = camera;
}

void aech::graphics::transparent_renderer_t::set_dirlight(directional_light_t* dirlight)
{
	m_dirlight = dirlight;
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

	m_mesh_filter.material()->shader()->use();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const static math::mat4_t bias_matrix
	{
		{0.5F, 0.0F, 0.0F, 0.5F},
		{0.0F, 0.5F, 0.0F, 0.5F},
		{0.0F, 0.0F, 0.5F, 0.5F},
		{0.0F, 0.0F, 0.0F, 1.0F}
	};
	
	auto light_view = math::get_view_matrix(*m_dirlight_transform);

	if (renderer.shadows()) 
	{
		m_mesh_filter.material()->shader()->set_uniform("poisson_sampling_distance_multiplier", renderer.poisson_sampling_distance());
	}

	for (auto entity : entities)
	{
		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
		auto& transform   = engine.get_component<transform_t>(entity);
		auto& scene_node  = engine.get_component<scene_node_t>(entity);

		mesh_filter.material()->set_uniforms();
		mesh_filter.material()->shader()->set_uniform("model", transform.get_transform_matrix());
		mesh_filter.material()->shader()->set_uniform("view", math::get_view_matrix(*m_camera_transform));
		mesh_filter.material()->shader()->set_uniform("projection", m_camera->projection);
		mesh_filter.material()->shader()->set_uniform("light_dir", m_dirlight_transform->get_forward_vector());
		mesh_filter.material()->shader()->set_uniform("light_colour", m_dirlight->colour);
		mesh_filter.material()->shader()->set_uniform("light_intensity", m_dirlight->intensity);
		mesh_filter.material()->shader()->set_uniform("depth_bias_vp", bias_matrix * aech::graphics::renderer_t::
		                                                               light_projection * light_view);
		mesh_filter.material()->shader()->set_uniform("camera_position", m_camera_transform->position);
		mesh_filter.mesh()->draw();
	}
}
