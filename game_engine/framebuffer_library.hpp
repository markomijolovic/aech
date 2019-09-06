#pragma once
#include <unordered_map>
#include <string>
#include "framebuffer.hpp"

namespace aech::graphics
{
	inline std::unordered_map<std::string, framebuffer_t> framebuffers{};

	void generate_default_framebuffers();
}
