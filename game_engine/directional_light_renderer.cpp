#include "directional_light_renderer.hpp"
#include "directional_light.hpp"

void aech::directional_light_renderer_t::update()
{
	mesh_filter.material->m_shader->use();

	glViewport(0, 0, screen_width, screen_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	for (auto light : entities)
	{
		auto& transform = engine.get_component<transform_t>(light);
		auto& directional_light = engine.get_component<directional_light_t>(light);

		{
			// shadows : light space projection
		}

		mesh_filter.material->m_shader->set_uniform("light_dir", transform.get_forward_vector());
		mesh_filter.material->m_shader->set_uniform("light_colour", directional_light.colour);
		mesh_filter.material->m_shader->set_uniform("light_intensity", directional_light.intensity);
		mesh_filter.material->set_uniforms();

		glBindVertexArray(mesh_filter.mesh->m_vao);
		// TODO: use glDrawElements
		glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh_filter.mesh->m_positions.size());
		glBindVertexArray(0);
	}
}
