#include "renderer.hpp"
#include "transforms.hpp"
#include "main.hpp"
#include "scene_node.hpp"
#include "mesh_library.hpp"
#include "transform.hpp"
#include "mesh_filter.hpp"

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
			camera_t::make_perspective_projection(45.0f, 0.1f, 1000.0f, 1280, 720)
			}
		);

		material_library::generate_default_materials();
		mesh_library::generate_default_meshes();
	}

	void renderer_t::update(float delta_time)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
			shader->use();
			glBindVertexArray(mesh_filter.mesh->m_vao);
			// auto const& renderable = engine.get_component<renderable_t>(entity);

			/*mat4_t view;
			view.data[0][3] = -cameraTransform.position.x;
			view.data[1][3] = -cameraTransform.position.y;
			view.data[2][3] = -cameraTransform.position.z;*/

			mat4_t rotY;

			float cos_theta_y = cosf(transform.rotation.y);
			float sin_theta_y = sinf(transform.rotation.y);

			rotY.data[0][0] = cos_theta_y;
			rotY.data[2][0] = -sin_theta_y;
			rotY.data[0][2] = sin_theta_y;
			rotY.data[2][2] = cos_theta_y;


			mat4_t rotX;

			float cosThetaX = cosf(transform.rotation.x);
			float sinThetaX = sinf(transform.rotation.x);

			rotX.data[1][1] = cosThetaX;
			rotX.data[2][1] = sinThetaX;
			rotX.data[1][2] = -sinThetaX;
			rotX.data[2][2] = cosThetaX;


			mat4_t rotZ;

			float cosThetaZ = cosf(transform.rotation.z);
			float sinThetaZ = sinf(transform.rotation.z);



			rotZ.data[0][0] = cosThetaZ;
			rotZ.data[1][0] = sinThetaZ;
			rotZ.data[0][1] = -sinThetaZ;
			rotZ.data[1][1] = cosThetaZ;


			mat4_t translate;
			translate.data[0][3] = transform.position.x;
			translate.data[1][3] = transform.position.y;
			translate.data[2][3] = transform.position.z;



			mat4_t scaleMat;
			scaleMat.data[0][0] = transform.scale.x;
			scaleMat.data[1][1] = transform.scale.y;
			scaleMat.data[2][2] = transform.scale.z;

			mat4_t model = translate * scaleMat * rotZ * rotY * rotX;

			mat4_t projection = camera.projection;

			shader->set_uniform("model", model);
			shader->set_uniform("view", view);
			shader->set_uniform("projection", projection);
			// shader->set_uniform("colour", std::get<vec4_t>(mesh_filter.material->m_uniforms["colour"].value));
			// shader->set_uniform("uColor", renderable.colour);

			glDrawElements(GL_TRIANGLES, mesh_filter.mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

	}
}
