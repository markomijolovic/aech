#include "main.hpp"
#include "aech_types.hpp"
#include "input_manager.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "scene_node.hpp"
#include <chrono>

int main()
{
    using namespace aech;
    using namespace graphics;
    using namespace events;

    engine.init();
    renderer.init();

    auto root_node = resource_manager::load_mesh("textures_pbr/sponza.obj");
    auto scene_node = engine.get_component<scene_node_t>(root_node);
    scene_node.set_scale(0.01F);
    renderer.bake_probes();
    auto delta_time = 1 / 60.0F;
    auto now = std::chrono::high_resolution_clock::now();

    while (!window_manager.should_close()) {
        window_manager.begin_frame();

        delta_time = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - now).count();
        now = std::chrono::high_resolution_clock::now();

        input_manager.update(delta_time);
        renderer.update();

        window_manager.end_frame();
    }
}
