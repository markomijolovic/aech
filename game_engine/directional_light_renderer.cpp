#include "directional_light_renderer.hpp"
#include "directional_light.hpp"

#include "mesh_filter.hpp"

#include "shader.hpp"

#include "transforms.hpp"

#include "main.hpp"


void aech::graphics::directional_light_renderer_t::update()
{
	mesh_filter.material()->shader()->use();

	render_target->bind();
	glViewport(0, 0, window_manager.width(), window_manager.height());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	auto         light_projection = math::orthographic(-2250, 2250, -2250, 2000, 0, 2250);
	math::mat4_t bias_matrix
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

	if (renderer.shadows)
	{
		mesh_filter.material()->shader()->set_uniform("poisson_sampling_distance_multiplier", renderer.poisson_sampling_distance);
	}
	
	for (auto light : entities)
	{
		auto& transform         = engine.get_component<transform_t>(light);
		auto& directional_light = engine.get_component<directional_light_t>(light);

		auto light_view = math::get_view_matrix(transform);

		mesh_filter.material()->shader()->set_uniform("light_dir", transform.get_forward_vector());
		mesh_filter.material()->shader()->set_uniform("light_colour", directional_light.colour);
		mesh_filter.material()->shader()->set_uniform("light_intensity", directional_light.intensity);
		mesh_filter.material()->shader()->set_uniform("depth_bias_vp", bias_matrix * light_projection * light_view);
		mesh_filter.material()->set_uniforms();

		mesh_filter.mesh()->draw();
	}
}
