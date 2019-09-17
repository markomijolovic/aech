#include "point_light_renderer.hpp"
#include "point_light.hpp"
#include "mesh_filter.hpp"
#include "transforms.hpp"
#include "camera.hpp"
#include "main.hpp"

void aech::graphics::point_light_renderer_t::update()
{
	auto& camera_transform = engine.get_component<transform_t>(m_camera);
	auto& camera = engine.get_component<camera_t>(m_camera);

	for (auto light: entities)
	{
		auto& transform = engine.get_component<transform_t>(light);
		auto& mesh_filter = engine.get_component<mesh_filter_t>(light);
		auto& point_light = engine.get_component<point_light_t>(light);

		mesh_filter.material()->shader()->use();
		mesh_filter.material()->set_uniforms();

		mesh_filter.material()->shader()->set_uniform("camera_position", camera_transform.position);
		mesh_filter.material()->shader()->set_uniform("light_position", transform.position);
		mesh_filter.material()->shader()->set_uniform("light_range", point_light.range);
		mesh_filter.material()->shader()->set_uniform("light_colour", point_light.colour);
		mesh_filter.material()->shader()->set_uniform("light_intensity", point_light.intensity);
		mesh_filter.material()->shader()->set_uniform("model", transform.get_transform_matrix());
		mesh_filter.material()->shader()->set_uniform("view", math::get_view_matrix(camera_transform));
		mesh_filter.material()->shader()->set_uniform("projection", camera.projection);
	}
}
