#pragma once
#include "shader.hpp"

#include <array>

#include <glad/glad.h>


namespace aech::graphics
{
	enum class cull_face
	{
		front = GL_FRONT,
		back = GL_BACK,
		front_and_back = GL_FRONT_AND_BACK
	};

	enum class depth_func
	{
		less = GL_LESS,
		equal = GL_EQUAL,
		lequal = GL_LEQUAL,
	};

	enum class blend_func
	{
		src_alpha = GL_SRC_ALPHA,
		one_minus_src_alpha = GL_ONE_MINUS_SRC_ALPHA,
		one = GL_ONE
	};

	enum class clear
	{
		color_buffer_bit = GL_COLOR_BUFFER_BIT,
		depth_buffer_bit = GL_DEPTH_BUFFER_BIT,
		color_and_depth_buffer_bit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
	};

	// a class that only changes render state
	// when needed
	class render_cache_t
	{
	public:
		render_cache_t();

		void        set_depth_test(bool enable);
		void        set_depth_func(depth_func func);
		void        set_blend(bool enable);
		void        set_blend(blend_func source, blend_func dest);
		void        set_cull(bool enable);
		void        set_cull_face(cull_face face);
		void        set_shader(shader_t* shader);
		void        set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void clear(clear bit);

	private:
		bool       m_depth_test{true};
		depth_func m_depth_func{depth_func::less};
		bool       m_blend{true};
		blend_func m_blend_source{blend_func::src_alpha};
		blend_func m_blend_dest{blend_func::one_minus_src_alpha};
		bool       m_cull{true};
		cull_face  m_cull_face{cull_face::back};
		shader_t*  m_shader{nullptr};

		std::array<uint32_t, 4> viewport_dimensions{};
	};
} // namespace aech::graphics
