#pragma once

#include <bitset>

#include <cstdint>

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

	using event_id_t = std::uint32_t;
	using param_id_t = std::uint32_t;
	using entity_t = std::uint32_t;
	using component_type_t = std::uint8_t;
	constexpr entity_t         max_entities   = 20000;
	constexpr component_type_t max_components = 32;
	using signature_t = std::bitset<max_components>;

	static constexpr float pi = 3.1415926535897932384626433832795029;

	enum class input_buttons
	{
		w,
		a,
		s,
		d,
		q,
		e
	};

	constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
	{
		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
	}

	constexpr uint32_t operator""_hash (const char *s, std::size_t count)
	{
		return fnv1a_32(s, count);
	}

	namespace events::window
	{
		constexpr event_id_t input = "Events::Window::INPUT"_hash;
	}

	namespace events::window::params
	{
		constexpr param_id_t input = "Events::Window::Params::INPUT"_hash;
	}
} // namespace aech
