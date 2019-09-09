#pragma once

#include "aech_types.hpp"
#include "texture_types.hpp"

namespace aech::graphics
{
	class texture_cube_t
	{
	public:
		uint32_t id{};
		uint32_t width{};
		uint32_t height{};
		texture_types::sized_internal_format sized_internal_format =	texture_types::sized_internal_format::rgba8;
		texture_types::format format = texture_types::format::rgba;
		texture_types::type type = texture_types::type::ubyte;
		texture_types::filtering filtering_mag = texture_types::filtering::linear;
		texture_types::filtering filtering_min = texture_types::filtering::linear;
		texture_types::sampling wrap_s = texture_types::sampling::clamp_to_edge;
		texture_types::sampling wrap_t = texture_types::sampling::clamp_to_edge;
		texture_types::sampling wrap_r = texture_types::sampling::clamp_to_edge;
		texture_types::target target = texture_types::target::cube_map;
		bool mipmap = true;
		void* data = nullptr;

		//default init faces
		void init();
		void generate_face(uint32_t index,
			void* data);
		void generate_mips();

		void bind(int32_t unit = -1) const;
		void unbind() const;
	};
}
