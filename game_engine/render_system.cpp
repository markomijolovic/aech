#include "render_system.hpp"
#include "engine.hpp"
#include "components.hpp"
#include "camera.hpp"

extern aech::engine_t engine;

namespace aech
{
	void render_system_t::init()
	{
		shader = std::make_unique<shader_t>("./vertex.glsl", "./fragment.glsl");

		m_camera = engine.create_entity();

		engine.add_component(
			m_camera,
			transform_t{
				{0.0f, 0.0f, 400.0f} 
			}
		);

		engine.add_component(
			m_camera,
			camera_t{
			camera_t::make_projection_matrix(45.0f, 0.1f, 1000.0f, 1280, 720)
			}
		);

		std::vector<vec3_t> vertices =
		{
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(0.5f, -0.5f, -0.5f),
			vec3_t(0.5f, 0.5f, -0.5f),
			vec3_t(0.5f, 0.5f, -0.5f),
			vec3_t(-0.5f, 0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, 0.5),
			vec3_t(0.5f, -0.5f, 0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, -0.5f, 0.5),
			vec3_t(-0.5f, 0.5f, 0.5f),
			vec3_t(-0.5f, 0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(-0.5f, -0.5f, 0.5f),
			vec3_t(-0.5f, 0.5f, 0.5f),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(0.5f, 0.5f, -0.5),
			vec3_t(0.5f, -0.5f, -0.5),
			vec3_t(0.5f, -0.5f, -0.5),
			vec3_t(0.5f, -0.5f, 0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(0.5f, -0.5f, -0.5f),
			vec3_t(0.5f, -0.5f, 0.5f),
			vec3_t(0.5f, -0.5f, 0.5f),
			vec3_t(-0.5f, -0.5f, 0.5f),
			vec3_t(-0.5f, -0.5f, -0.5f),
			vec3_t(-0.5f, 0.5f, -0.5),
			vec3_t(0.5f, 0.5f, -0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, 0.5f, 0.5),
			vec3_t(-0.5f, 0.5f, -0.5)
		};

		std::vector<vec3_t> normals =
		{
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, -1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(0.0f, 0.0f, 1.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(-1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(1.0f, 0.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, -1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0f),
			vec3_t(0.0f, 1.0f, 0.0)
		};

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3_t)* vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_t), nullptr);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &m_vbo_normals);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3_t)* normals.size(), normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3_t), nullptr);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void render_system_t::update(float dt)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		shader->use();
		glBindVertexArray(m_vao);


		auto& cameraTransform = engine.get_component<transform_t>(m_camera);
		auto& camera = engine.get_component<camera_t>(m_camera);

		for (auto const& entity : m_entities)
		{
			auto const& transform = engine.get_component<transform_t>(entity);
			auto const& renderable = engine.get_component<renderable_t>(entity);

			mat4_t view;
			view.data[0][3] = -cameraTransform.position.x;
			view.data[1][3] = -cameraTransform.position.y;
			view.data[2][3] = -cameraTransform.position.z;

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

			mat4_t model = translate * scaleMat * rotY;

			mat4_t projection = camera.projection;

			shader->set_uniform<mat4_t>("uModel", model);
			shader->set_uniform<mat4_t>("uView", view);
			shader->set_uniform<mat4_t>("uProjection", projection);
			shader->set_uniform<vec3_t>("uColor", renderable.colour);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

	}
}
