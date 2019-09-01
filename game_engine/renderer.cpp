#include "renderer.hpp"
#include "transforms.hpp"
#include "main.hpp"
#include "scene_node.hpp"
#include "mesh_library.hpp"
#include "transform.hpp"
#include "mesh_filter.hpp"
#include <iostream>

namespace aech
{
	renderer_t::renderer_t()
		: m_camera{engine.create_entity()}
	{
		engine.add_component(
			m_camera,
			transform_t{
				{0.0f, 0.0f, 400.0f}
			}
		);

		engine.add_component(
			m_camera,
			camera_t{
			camera_t::make_perspective_projection(60.0f, 0.1f, 1000.0f, 1280, 720)
			}
		);

		material_library::generate_default_materials();
		mesh_library::generate_default_meshes();
	}

	void renderer_t::update(float delta_time)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		auto& cameraTransform = engine.get_component<transform_t>(m_camera);
		auto& camera = engine.get_component<camera_t>(m_camera);

		auto view = get_view_matrix(cameraTransform);

		for (auto const& entity : entities)
		{
			auto& transform = engine.get_component<transform_t>(entity);
			auto& scene_node = engine.get_component<scene_node_t>(entity);
			auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
			auto shader = mesh_filter.material->m_shader;

			auto model = scene_node.get_transform();

			mat4_t projection = camera.projection;

			shader->use();
			glBindVertexArray(mesh_filter.mesh->m_vao);
			mesh_filter.material->bind_textures();
			shader->set_uniform("model", model);
			shader->set_uniform("view", view);
			shader->set_uniform("projection", projection);
			shader->set_uniform("point_light_positions[0]", vec3_t{ 0, 50, 0 });
			shader->set_uniform("point_light_intensities[0]", vec3_t{ 1, 1, 1 });
			// shader->set_uniform("colour", std::get<vec4_t>(mesh_filter.material->m_uniforms["colour"].value));
			// shader->set_uniform("uColor", renderable.colour);

			glDrawElements(GL_TRIANGLES, mesh_filter.mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

	}
}
