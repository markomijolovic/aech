#include "glad/glad.h"
#include "aech_types.hpp"
#include "camera.hpp"
#include "directional_light.hpp"
#include "mesh_filter.hpp"
#include "point_light.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "scene_node.hpp"
#include "shading_tags.hpp"
#include "shadow_caster.hpp"
#include <chrono>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "input_manager.hpp"
#include "main.hpp"

int main(int /*argc*/, char* /*argv*/[])
{
	using namespace aech;
	using namespace graphics;
	using namespace events;
	
	engine.init();
	renderer.init();

	auto root_node = resource_manager::load_mesh("textures_pbr/sponza.obj");
	auto scene_node = engine.get_component<scene_node_t>(root_node);
	scene_node.set_scale(0.1F);
	auto delta_time = 1 / 60.0F;
	renderer.bake_probes();

	while (!window_manager.should_close())
	{
		window_manager.begin_frame();

		auto start_time = std::chrono::high_resolution_clock::now();

		input_manager.update(delta_time);
		renderer.update();
		auto stop_time = std::chrono::high_resolution_clock::now();
		delta_time     = std::chrono::duration<float>(stop_time - start_time).count();

		window_manager.end_frame();
	}
}