#include "glad/glad.h"
#include "imgui.h"

#include "imgui_impl_glfw.h"

#include "imgui_impl_opengl3.h"

#include "main.hpp"

#include "window_manager.hpp"

#include <chrono>

static void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);
static void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);

uint32_t aech::graphics::window_manager_t::width() const
{
	return m_screen_width;
}

uint32_t aech::graphics::window_manager_t::height() const
{
	return m_screen_height;
}

bool aech::graphics::window_manager_t::should_close() const
{
	return glfwWindowShouldClose(m_window) != 0;
}

aech::graphics::window_manager_t::~window_manager_t()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}


void aech::graphics::window_manager_t::begin_frame() const
{
	glfwPollEvents();
}

void aech::graphics::window_manager_t::end_frame() const
{
	glfwSwapBuffers(m_window);
}

aech::graphics::window_manager_t::window_manager_t()
{
	if (glfwInit() == 0)
	{
		throw std::exception{};
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	m_window = glfwCreateWindow(m_screen_width, m_screen_height, "aech", nullptr, nullptr);
	if (m_window == nullptr)
	{
		throw std::exception{};
	}

	glfwMakeContextCurrent(m_window);
	// disable vsync
	glfwSwapInterval(0);

	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		throw std::exception{};
	}

	glfwSetKeyCallback(m_window, &key_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glViewport(0, 0, m_screen_width, m_screen_height);
	glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

	// init dear imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsLight();
	auto& io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

static void key_callback(GLFWwindow* window, int key, int /*scan_code*/, int action, int /*mode*/)
{
	using namespace aech;
	using namespace graphics;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, 1);
		return;
	}


	if (action == GLFW_PRESS)
	{
		auto button_pressed = false;

		switch (key)
		{
		case GLFW_KEY_W:

			window_manager.set_button(input_buttons::w);
			button_pressed = true;
			break;
		case GLFW_KEY_A:
			window_manager.set_button(input_buttons::a);
			button_pressed = true;
			break;
		case GLFW_KEY_S:
			window_manager.set_button(input_buttons::s);
			button_pressed = true;
			break;
		case GLFW_KEY_D:
			window_manager.set_button(input_buttons::d);
			button_pressed = true;
			break;
		case GLFW_KEY_Q:
			window_manager.set_button(input_buttons::q);
			button_pressed = true;
			break;
		case GLFW_KEY_E:
			window_manager.set_button(input_buttons::e);
			button_pressed = true;
			break;
		case GLFW_KEY_O:
			window_manager.set_button(input_buttons::o);
			if (!renderer.options())
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			button_pressed = true;
			break;
		default:
			// do nothing
			break;
		}

		if (button_pressed)
		{
			events::event_t event{events::window::keyboard};
			event.set_param(events::window::params::keyboard, window_manager.buttons());
			engine.send_event(event);
		}
	}
	else if (action == GLFW_RELEASE)
	{
		auto button_released = false;

		switch (key)
		{
		case GLFW_KEY_W:
			window_manager.reset_button(input_buttons::w);
			button_released = true;
			break;
		case GLFW_KEY_A:
			window_manager.reset_button(input_buttons::a);
			button_released = true;
			break;
		case GLFW_KEY_S:
			window_manager.reset_button(input_buttons::s);
			button_released = true;
			break;
		case GLFW_KEY_D:
			window_manager.reset_button(input_buttons::d);
			button_released = true;
			break;
		case GLFW_KEY_Q:
			window_manager.reset_button(input_buttons::q);
			button_released = true;
			break;
		case GLFW_KEY_E:
			window_manager.reset_button(input_buttons::e);
			button_released = true;
			break;
		case GLFW_KEY_O:
			window_manager.reset_button(input_buttons::o);
			button_released = true;
			break;
		default:
			// do nothing
			break;
		}

		if (button_released)
		{
			events::event_t event{events::window::keyboard};
			event.set_param(events::window::params::keyboard, window_manager.buttons());
			engine.send_event(event);
		}
	}
}

static void mouse_callback(GLFWwindow* /*window*/, double x_pos, double y_pos)
{
	using namespace aech;
	using namespace graphics;
	if (window_manager.first_mouse())
	{
		window_manager.set_x(static_cast<float>(x_pos));
		window_manager.set_y(static_cast<float>(y_pos));
		window_manager.set_first_mouse(false);
	}

	auto x_offset = static_cast<float>(x_pos) - window_manager.x();
	auto y_offset = window_manager.y() - static_cast<float>(y_pos);

	window_manager.set_x(static_cast<float>(x_pos));
	window_manager.set_y(static_cast<float>(y_pos));

	std::pair<float, float> param = {x_offset, y_offset};
	events::event_t event{events::window::mouse};
	event.set_param(events::window::params::mouse, param);
	engine.send_event(event);
}


void aech::graphics::window_manager_t::set_button(input_buttons button)
{
	m_buttons.set(static_cast<size_t>(button));
}

void aech::graphics::window_manager_t::set_x(float x)
{
	m_last_x = x;
}

void aech::graphics::window_manager_t::set_y(float y)
{
	m_last_y = y;
}

std::bitset<32> aech::graphics::window_manager_t::buttons() const
{
	return m_buttons;
}

float aech::graphics::window_manager_t::x() const
{
	return m_last_x;
}

float aech::graphics::window_manager_t::y() const
{
	return m_last_y;
}

bool aech::graphics::window_manager_t::first_mouse() const
{
	return m_first_mouse;
}

void aech::graphics::window_manager_t::set_first_mouse(bool f)
{
	m_first_mouse = f;
}


void aech::graphics::window_manager_t::reset_button(input_buttons button)
{
	m_buttons.reset(static_cast<size_t>(button));
}
