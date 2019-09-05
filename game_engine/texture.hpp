#pragma once
#include "aech_types.hpp"
#include <glad/glad.h>

namespace aech::graphics
{
	namespace texture_types
	{
		enum class sized_internal_format
		{
			r8 = GL_R8,
			rg8 = GL_RG8,
			rgb8 = GL_RGB8,
			rgba8 = GL_RGBA8,
			r32f = GL_R32F,
			rg32f = GL_RG32F,
			rgb32f = GL_RGB32F,
			rgba32f = GL_RGBA32F,
			depth24 = GL_DEPTH_COMPONENT24
		};

		enum class format
		{
			r = GL_RED,
			rg = GL_RG,
			rgb = GL_RGB,
			rgba = GL_RGBA,
			depth = GL_DEPTH_COMPONENT
		};

		enum class type
		{
			ubyte = GL_UNSIGNED_BYTE,
			floating_point = GL_FLOAT,
			uint8888 = GL_UNSIGNED_INT_8_8_8_8
		};

		enum class filtering
		{
			linear = GL_LINEAR,
			linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR,
			nearest = GL_NEAREST
		};

		enum class sampling
		{
			repeat = GL_REPEAT
		};

		enum class target
		{
			oned = GL_TEXTURE_1D,
			twod = GL_TEXTURE_2D,
			threed = GL_TEXTURE_3D,
			cube_map = GL_TEXTURE_CUBE_MAP
		};
	}


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
		texture_types::sampling wrap_r = texture_types::sampling::repeat;
		texture_types::target target = texture_types::target::twod;
		bool mipmap = true;
		void* data = nullptr;

		// creates an immutable texture
		void generate();

		void bind(int32_t = -1) const;
		void unbind() const;
	};
}
