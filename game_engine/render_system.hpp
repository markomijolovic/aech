#pragma once
#include "system.hpp"
#include "shader.hpp"
#include "components.hpp"

namespace aech
{

	class render_system_t : public system_t {
	public:
		void init();
		void update(float dt);

	private:
		shader_t* shader;
		entity_t m_camera;

		GLuint m_vao{};
		GLuint m_vbo_vertices{};
		GLuint m_vbo_normals{};
	};
}
