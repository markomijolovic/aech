#include "directional_light_renderer.hpp"
#include "directional_light.hpp"
#include "transforms.hpp"
#include "main.hpp"

void aech::graphics::directional_light_renderer_t::update()
{
	mesh_filter.material->m_shader->use();

	glViewport(0, 0, screen_width, screen_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	auto light_projection = math::orthographic(-2250, 2250, -2250, 2000, 0, 2250);
	aech::math::mat4_t bias_matrix
	{
		0.5f, 0, 0, 0.5f,
		0, 0.5f, 0, 0.5f,
		0, 0, 0.5f, 0.5f,
		0, 0, 0, 1
	};
	for (auto light : entities)
	{
		auto& transform = engine.get_component<transform_t>(light);
		auto& directional_light = engine.get_component<directional_light_t>(light);

		auto light_view = math::get_view_matrix(transform);

		{
			// shadows : light space projection
		}

		mesh_filter.material->m_shader->set_uniform("light_dir", transform.get_forward_vector());
		mesh_filter.material->m_shader->set_uniform("light_colour", directional_light.colour);
		mesh_filter.material->m_shader->set_uniform("light_intensity", directional_light.intensity);
		mesh_filter.material->m_shader->set_uniform("depth_bias_vp", bias_matrix * light_projection * light_view);
		mesh_filter.material->set_uniforms();

		glBindVertexArray(mesh_filter.mesh->m_vao);
		// TODO: use glDrawElements
		glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh_filter.mesh->m_positions.size());
		glBindVertexArray(0);
	}
}
