#include "transparent_renderer.hpp"
#include "camera.hpp"

#include "directional_light.hpp"

#include "main.hpp"

#include "scene_node.hpp"

#include "shader.hpp"

#include "transform.hpp"

#include "transforms.hpp"


void graphics::transparent_renderer_t::update()
{
	render_target->bind();

	mesh_filter.material()->shader()->use();

	auto& camera_transform = engine.get_component<transform_t>(m_camera);
	auto& camera           = engine.get_component<camera_t>(m_camera);
	auto  view             = math::get_view_matrix(camera_transform);
	auto  projection       = camera.projection;

	auto& light_transform = engine.get_component<transform_t>(dirlight);
	auto& light           = engine.get_component<directional_light_t>(dirlight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// TODO(Marko): sync this across renderers
	auto         light_projection = math::orthographic(-2250, 2250, -2250, 2000, 0, 2250);
	math::mat4_t bias_matrix
	{
		0.5f,
		0,
		0,
		0.5f,
		0,
		0.5f,
		0,
		0.5f,
		0,
		0,
		0.5f,
		0.5f,
		0,
		0,
		0,
		1
	};

	auto light_view = math::get_view_matrix(light_transform);


	for (auto entity : entities)
	{
		auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
		auto& transform   = engine.get_component<transform_t>(entity);
		auto& scene_node  = engine.get_component<scene_node_t>(entity);

		mesh_filter.material()->set_uniforms();
		mesh_filter.material()->shader()->set_uniform("model", transform.get_transform_matrix());
		mesh_filter.material()->shader()->set_uniform("view", view);
		mesh_filter.material()->shader()->set_uniform("projection", projection);
		mesh_filter.material()->shader()->set_uniform("light_dir", light_transform.get_forward_vector());
		mesh_filter.material()->shader()->set_uniform("light_colour", light.colour);
		mesh_filter.material()->shader()->set_uniform("light_intensity", light.intensity);
		mesh_filter.material()->shader()->set_uniform("depth_bias_vp", bias_matrix * light_projection * light_view);
		mesh_filter.material()->shader()->set_uniform("camera_position", camera_transform.position);
		mesh_filter.mesh()->draw();
	}
}
