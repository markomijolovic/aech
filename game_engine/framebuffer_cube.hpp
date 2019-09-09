#pragma once

#include "texture_cube.hpp"

namespace aech::graphics
{
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
		void attach(int i) const;
		void bind() const;
		void unbind() const;
	};
}
