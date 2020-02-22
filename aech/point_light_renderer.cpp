#include "point_light_renderer.hpp"

#include "material.hpp"

#include "shader.hpp"

#include "camera.hpp"


#include "mesh_filter.hpp"

#include "point_light.hpp"

#include "main.hpp"

#include "transforms.hpp"


aech::graphics::point_light_renderer_t::point_light_renderer_t(render_cache_t* render_cache, camera_t* camera) :
	m_camera{camera},
	m_render_cache{render_cache}
{
}

void aech::graphics::point_light_renderer_t::update()
{
	for (auto light : entities)
	{
		auto& transform   = engine.get_component<transform_t>(light);
		auto& mesh_filter = engine.get_component<mesh_filter_t>(light);
		auto& point_light = engine.get_component<point_light_t>(light);
		auto& scene_node  = engine.get_component<scene_node_t>(light);

		m_render_cache->set_shader(mesh_filter.material()->shader());

		//mesh_filter.material()->shader()->use();
		mesh_filter.material()->set_uniforms();

		mesh_filter.material()->shader()->set_uniform("camera_position", m_camera->transform()->position);
		mesh_filter.material()->shader()->set_uniform("light_position", transform.position);
		mesh_filter.material()->shader()->set_uniform("light_range", point_light.range);
		mesh_filter.material()->shader()->set_uniform("light_colour", point_light.colour);
		mesh_filter.material()->shader()->set_uniform("light_intensity", point_light.intensity);
		mesh_filter.material()->shader()->set_uniform("model", scene_node.get_transform());
		mesh_filter.material()->shader()->set_uniform("view", math::get_view_matrix(*m_camera->transform()));
		mesh_filter.material()->shader()->set_uniform("projection", m_camera->projection());
	}
}
