#pragma once

#include "texture_cube.hpp"

namespace aech::graphics
{
	// TODO: should you be able to allocate an fbo on the stack?
	class framebuffer_cube_t
	{
	public:
		uint32_t id;
		uint32_t rbo_id;
		texture_cube_t* texture;
		uint32_t width{};
		uint32_t height{};

		framebuffer_cube_t() = default;
		framebuffer_cube_t(
			texture_cube_t*texture,
			uint32_t width,
			uint32_t height
			);
		void attach(int i, uint32_t miplevel = 0) const;
		void bind() const;
		void unbind() const;
	};
}
