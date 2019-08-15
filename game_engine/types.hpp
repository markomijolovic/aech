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

	using entity_t = std::uint32_t;
	using component_type_t = std::uint8_t;
	constexpr entity_t         max_entities   = 10000;
	constexpr component_type_t max_components = 32;
	using signature_t = std::bitset<max_components>;
} // namespace aech
