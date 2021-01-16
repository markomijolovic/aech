#pragma once

#include <bitset>
#include <cstdint>
#include <limits>

namespace aech {
// entity is just an ID
using entity_t = std::uint32_t;

// represents an invalid entity
inline constexpr entity_t invalid_entity_id = std::numeric_limits<entity_t>::max();

using component_type_t = std::uint8_t;

// maximum number of entities alive at one point in time
inline constexpr entity_t max_entities = 20000;

// maximum number of components in the engine
inline constexpr component_type_t max_components = 32;

// used to identify which components a system is interested in
using signature_t = std::bitset<max_components>;

inline constexpr float pi = 3.1415926535897932384626433832795029F;

using event_id_t = std::uint32_t;
using param_id_t = std::uint32_t;

enum class input_buttons {
    w,
    a,
    s,
    d,
    q,
    e,
    o
};

constexpr auto fnv1a_32(const char *s, std::size_t count) -> std::uint32_t
{
    return static_cast<std::uint32_t>(((count != 0U ? fnv1a_32(s, count - 1) : 2166136261U) ^ s[count]) * 16777619ULL);
}

constexpr auto operator""_hash(const char *s, std::size_t count) -> std::uint32_t
{
    return fnv1a_32(s, count);
}

namespace events::window {
constexpr event_id_t keyboard = "Events::Window::KEYBOARD"_hash;
constexpr event_id_t mouse    = "Events::Window::MOUSE"_hash;
} // namespace events::window

namespace events::window::params {
constexpr param_id_t keyboard = "Events::Window::Params::KEYBOARD"_hash;
constexpr param_id_t mouse{"Events::Window::Params::MOUSE"_hash};
} // namespace events::window::params
} // namespace aech
