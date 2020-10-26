#include "engine.hpp"

#include "camera.hpp"
#include "directional_light.hpp"
#include "light_probe.hpp"
#include "mesh_filter.hpp"
#include "point_light.hpp"
#include "reflection_probe.hpp"
#include "scene_node.hpp"
#include "shading_tags.hpp"
#include "shadow_caster.hpp"

namespace aech {
auto engine_t::init() -> void
{
    register_component<transform_t>();
    register_component<graphics::scene_node_t>();
    register_component<camera_t>();
    register_component<graphics::mesh_filter_t>();
    register_component<graphics::directional_light_t>();
    register_component<graphics::point_light_t>();
    register_component<graphics::potential_occluder_t>();
    register_component<graphics::opaque_t>();
    register_component<graphics::transparent_t>();
    register_component<graphics::reflection_probe_t>();
    register_component<graphics::light_probe_t>();
}

auto engine_t::create_entity() -> entity_t
{
    return m_entity_manager.create_entity();
}

auto engine_t::destroy_entity(entity_t entity) -> void
{
    m_entity_manager.destroy_entity(entity);
    m_component_manager.entity_destroyed(entity);
    m_system_manager.entity_destroyed(entity);
}

auto engine_t::set_root_node(entity_t root_node) -> void
{
    m_root_node = root_node;
}

auto engine_t::root_node() const -> entity_t
{
    return m_root_node;
}

auto engine_t::add_event_listener(event_id_t event_id, const std::function<void(events::event_t &)> &listener) -> void
{
    m_event_manager.add_listener(event_id, listener);
}

auto engine_t::send_event(events::event_t &event) -> void
{
    m_event_manager.send_event(event);
}
} // namespace aech
