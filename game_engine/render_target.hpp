#pragma once

#include <glad/glad.h>
#include "texture.hpp"
#include <vector>
#include <memory>

namespace aech::graphics
{
	class render_target_t
	{
	public:
		uint32_t id;
		uint32_t width;
		uint32_t height;

		std::unique_ptr<texture_t> m_depth_and_stencil_texture{};
		std::vector<texture_t> m_colour_attachments{};

		// this needs refactoring
		render_target_t(
			uint32_t width, uint32_t height, uint32_t num_colour_attachments, bool depth,
			texture_types::sized_internal_format sized_internal_format,
			texture_types::format format,
			texture_types::type type
		);
		void bind() const;
		void unbind() const;
	};
}
