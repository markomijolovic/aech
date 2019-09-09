#pragma once
#include <unordered_map>
#include <string>
#include "framebuffer.hpp"
#include "framebuffer_cube.hpp"

namespace aech::graphics
{
	inline std::unordered_map<std::string, framebuffer_t> framebuffers{};
	inline std::unordered_map<std::string, framebuffer_cube_t> framebuffer_cubes{};

	void generate_default_framebuffers();
}
