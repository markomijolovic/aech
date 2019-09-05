#pragma once
#include <cstdint>
#include <bitset>

namespace aech
{
	using std::int8_t;
	using std::int16_t;
	using std::int32_t;
	using std::int64_t;

	using std::uint8_t;
	using std::uint16_t;
	using std::uint32_t;
	using std::uint64_t;

	using entity_t = std::uint32_t;
	inline constexpr entity_t invalid_entity_id = std::numeric_limits<entity_t>::max();
	using component_type_t = std::uint8_t;
	inline constexpr entity_t         max_entities = 20000;
	inline constexpr component_type_t max_components = 32;
	using signature_t = std::bitset<max_components>;

	inline constexpr float pi = 3.1415926535897932384626433832795029;

	using event_id_t = std::uint32_t;
	using param_id_t = std::uint32_t;

	enum class input_buttons
	{
		w,
		a,
		s,
		d,
		q,
		e
	};

	inline constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
	{
		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
	}

	inline constexpr uint32_t operator""_hash(const char* s, std::size_t count)
	{
		return fnv1a_32(s, count);
	}

	namespace events::window
	{
		constexpr event_id_t input = "Events::Window::INPUT"_hash;
		constexpr event_id_t mouse = "Events::Window::MOUSE"_hash;
	}

	namespace events::window::params
	{
		constexpr param_id_t input = "Events::Window::Params::INPUT"_hash;
		constexpr param_id_t mouse{ "Events::Window::Params::MOUSE"_hash };
	}
}
