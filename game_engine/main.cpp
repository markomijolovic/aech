#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "types.hpp"

#include <random>

#include <chrono>
#include "engine.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "physics_system.hpp"
#include "camera_control_system.hpp"
#include "render_system.hpp"
#include "player_control_system.hpp"

using namespace aech;

engine_t engine{};
std::bitset<8> m_buttons{};

constexpr uint32_t screen_width = 1920;
constexpr uint32_t screen_height = 1080;

auto first_mouse = true;
float last_x{};
float last_y{};

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);

int main(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	auto window = glfwCreateWindow(screen_width, screen_height, "ecs test", glfwGetPrimaryMonitor(), nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	glewInit();
	glGetError();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, screen_width, screen_height);

	glEnable(GL_DEPTH_TEST);

	//init
	engine.init();

	engine.register_component<camera_t>();
	engine.register_component<gravity_t>();
	engine.register_component<player_t>();
	engine.register_component<renderable_t>();
	engine.register_component<rigid_body_t>();
	engine.register_component<thrust_t>();
	engine.register_component<transform_t>();

	auto physics_system = engine.register_system<physics_system_t>();
	{
		signature_t signature{};
		signature.set(engine.get_component_type<gravity_t>());
		signature.set(engine.get_component_type<rigid_body_t>());
		signature.set(engine.get_component_type<transform_t>());
		engine.set_system_signature<physics_system_t>(signature);
	}
	physics_system->init();

	auto camera_control_system = engine.register_system<camera_control_system_t>();
	{
		signature_t signature{};
		signature.set(engine.get_component_type<camera_t>());
		signature.set(engine.get_component_type<transform_t>());
		engine.set_system_signature<camera_control_system_t>(signature);
	}
	camera_control_system->init();

	auto player_control_system = engine.register_system<player_control_system_t>();
	{
		signature_t signature{};
		signature.set(engine.get_component_type<player_t>());
		signature.set(engine.get_component_type<transform_t>());
		engine.set_system_signature<player_control_system_t>(signature);
	}
	player_control_system->init();

	auto render_system = engine.register_system<render_system_t>();
	{
		signature_t signature{};
		signature.set(engine.get_component_type<renderable_t>());
		signature.set(engine.get_component_type<transform_t>());
		engine.set_system_signature<render_system_t>(signature);
	}
	render_system->init();

	std::vector<entity_t> entities(max_entities-1);

	std::default_random_engine generator{};
	std::uniform_real_distribution<float> rand_position(-100.0f, 100.0f);
	std::uniform_real_distribution<float> rand_rotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> rand_scale(3.0f, 5.0f);
	std::uniform_real_distribution<float> rand_colour(0.0f, 1.0f);
	std::uniform_real_distribution<float> rand_gravity(-10.0f, -1.0f);
	auto scale = rand_scale(generator);

	for (auto &entity: entities)
	{
		entity = engine.create_entity();
		engine.add_component(entity, player_t{});

		engine.add_component(
			entity,
			gravity_t{ 
				vec3_t{0.0f, rand_gravity(generator), 0.0f}
			}
		);

		engine.add_component(
			entity,
			rigid_body_t{}
		);

		engine.add_component(
			entity,
			transform_t{
				{rand_position(generator),rand_position(generator) + 100.0f,rand_position(generator)}
				,{rand_rotation(generator),rand_rotation(generator),rand_rotation(generator)},
			{scale, scale, scale}
			}
		);

		engine.add_component(
			entity,
			renderable_t{
				{rand_colour(generator), rand_colour(generator), rand_colour(generator)} 
			}
		);
	}

	auto delta_time = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		glfwPollEvents();

		player_control_system->update(delta_time);
		camera_control_system->update(delta_time);
		physics_system->update(delta_time);
		render_system->update(delta_time);
		
		auto stop_time = std::chrono::high_resolution_clock::now();
		delta_time = std::chrono::duration<float, std::chrono::seconds::period>(stop_time - start_time).count();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return;
	}


	if (action == GLFW_PRESS)
	{
		auto button_pressed = false;

		switch(key)
		{
		case GLFW_KEY_W:
			m_buttons.set(static_cast<size_t>(input_buttons::w));
			button_pressed = true;
			break;
		case GLFW_KEY_A:
			m_buttons.set(static_cast<size_t>(input_buttons::a));
			button_pressed = true;
			break;
		case GLFW_KEY_S:
			m_buttons.set(static_cast<size_t>(input_buttons::s));
			button_pressed = true;
			break;
		case GLFW_KEY_D:
			m_buttons.set(static_cast<size_t>(input_buttons::d));
			button_pressed = true;
			break;
		case GLFW_KEY_Q:
			m_buttons.set(static_cast<size_t>(input_buttons::q));
			button_pressed = true;
			break;
		case GLFW_KEY_E:
			m_buttons.set(static_cast<size_t>(input_buttons::e));
			button_pressed = true;
			break;
		default:
			// do nothing
			break;
		}

		if (button_pressed)
		{
			event_t event{ events::window::input };
			event.set_param(events::window::params::input, m_buttons);
			engine.send_event(event);
		}
	}
	else if (action == GLFW_RELEASE)
	{
		auto button_released = false;

		switch (key)
		{
		case GLFW_KEY_W:
			m_buttons.reset(static_cast<size_t>(input_buttons::w));
			button_released = true;
			break;
		case GLFW_KEY_A:
			m_buttons.reset(static_cast<size_t>(input_buttons::a));
			button_released = true;
			break;
		case GLFW_KEY_S:
			m_buttons.reset(static_cast<size_t>(input_buttons::s));
			button_released = true;
			break;
		case GLFW_KEY_D:
			m_buttons.reset(static_cast<size_t>(input_buttons::d));
			button_released = true;
			break;
		case GLFW_KEY_Q:
			m_buttons.reset(static_cast<size_t>(input_buttons::q));
			button_released = true;
			break;
		case GLFW_KEY_E:
			m_buttons.reset(static_cast<size_t>(input_buttons::e));
			button_released = true;
			break;
		default:
			// do nothing
			break;
		}

		if (button_released)
		{
			event_t event{ events::window::input };
			event.set_param(events::window::params::input, m_buttons);
			engine.send_event(event);
		}
	}
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (first_mouse)
	{
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}

	float x_offset = x_pos - last_x;
	float y_offset = last_y - y_pos;

	last_x = x_pos;
	last_y = y_pos;

	std::pair param = { x_offset, y_offset };
	event_t event{ events::window::mouse };
	event.set_param(events::window::params::mouse, param);
	engine.send_event(event);
}