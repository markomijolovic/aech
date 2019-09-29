#pragma once
#include "aech_types.hpp"

#include "GLFW/glfw3.h"

namespace aech::graphics
{
	class window_manager_t
	{
	public:
		window_manager_t();
		~window_manager_t();

		bool should_close();
		void begin_frame();
		void end_frame();

		std::bitset<32> buttons();
		bool            first_mouse();
		float           x();
		float           y();

		void set_button(input_buttons button);
		void reset_button(input_buttons button);
		void set_first_mouse(bool f);
		void set_x(float x);
		void set_y(float y);

		uint32_t width();
		uint32_t height();

	private:
		bool            m_first_mouse{true};
		float           last_x{};
		float           last_y{};
		uint32_t        screen_width  = 1920;
		uint32_t        screen_height = 1080;
		std::bitset<32> m_buttons{};
		GLFWwindow*     window{};
	};
}
