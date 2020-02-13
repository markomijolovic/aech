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

		[[nodiscard]] bool should_close() const;
		static void        begin_frame();
		void               end_frame() const;

		[[nodiscard]] std::bitset<32> buttons() const;
		[[nodiscard]] bool            first_mouse() const;
		[[nodiscard]] float           x() const;
		[[nodiscard]] float           y() const;

		void set_button(input_buttons button);
		void reset_button(input_buttons button);
		void set_first_mouse(bool f);
		void set_x(float x);
		void set_y(float y);

		[[nodiscard]] uint32_t width() const;
		[[nodiscard]] uint32_t height() const;

	private:
		bool            m_first_mouse{true};
		float           last_x{};
		float           last_y{};
		uint32_t        screen_width  = 1920;
		uint32_t        screen_height = 800;
		std::bitset<32> m_buttons{};
		GLFWwindow*     window{};
	};
} // namespace aech::graphics
