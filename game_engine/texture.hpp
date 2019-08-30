#pragma once
#include "types.hpp"
#include <glad/glad.h>

namespace aech
{
	class texture_t
	{
	public:
		uint32_t m_id{};

		texture_t() = default;
		texture_t(uint32_t width, uint32_t height, GLenum internal_format, GLenum format, void *data);
		~texture_t() = default;

		void generate(uint32_t width, uint32_t height, GLenum internal_format, GLenum format, GLenum type, void* data);
		void bind(int32_t = -1) const;
		void unbind() const;
	};
}
