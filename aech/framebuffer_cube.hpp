#pragma once

#include "texture_cube.hpp"

namespace aech::graphics
{
	// TODO: should you be able to allocate a framebuffer on the stack
	class framebuffer_cube_t
	{
	public:
		framebuffer_cube_t() = default;
		framebuffer_cube_t(
			texture_cube_t* texture,
			uint32_t        width,
			uint32_t        height
		);

		void        attach(uint32_t i, uint32_t miplevel = 0) const;
		void        bind() const;
		static void unbind();

		[[nodiscard]] uint32_t        width() const;
		[[nodiscard]] uint32_t        height() const;
		[[nodiscard]] texture_cube_t* texture() const;
	private:
		uint32_t        id{};
		uint32_t        rbo_id{};
		texture_cube_t* m_texture{};
		uint32_t        m_width{};
		uint32_t        m_height{};
	};
} // namespace aech::graphics
