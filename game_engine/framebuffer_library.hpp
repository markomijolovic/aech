#pragma once
#include <unordered_map>
#include <string>
#include "render_target.hpp"

namespace aech::graphics
{
	inline std::unordered_map<std::string, render_target_t> framebuffers{};

	void generate_default_framebuffers();
}
