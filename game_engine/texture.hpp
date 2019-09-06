#pragma once

#include "texture_types.hpp"
#include "aech_types.hpp"

namespace aech::graphics
{
	class texture_t
	{
	public:
		uint32_t id{};
		uint32_t width{};
		uint32_t height{};
		uint32_t depth{};
		texture_types::sized_internal_format sized_internal_format = texture_types::sized_internal_format::rgba8;
		texture_types::format format = texture_types::format::rgba;
		texture_types::type type = texture_types::type::ubyte;
		texture_types::filtering filtering_mag = texture_types::filtering::linear;
		texture_types::filtering filtering_min = texture_types::filtering::linear;
		texture_types::sampling wrap_s = texture_types::sampling::repeat;
		texture_types::sampling wrap_t = texture_types::sampling::repeat;
		texture_types::target target = texture_types::target::twod;
		bool mipmap = true;
		void* data = nullptr;

		// creates an immutable texture
		void generate();

		void bind(int32_t = -1) const;
		void unbind() const;
	};
}
