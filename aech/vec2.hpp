#pragma once

namespace aech::math
{
	// two component vector
	class vec2_t
	{
	public:
		float x{};
		float y{};

		// initialize all components to zero
		vec2_t() = default;

		// initialize from the given scalars
		vec2_t(float x, float y);

		vec2_t& operator+=(const vec2_t& vec2);
		vec2_t& operator-=(const vec2_t& vec2);
		vec2_t& operator*=(float rhs);

		// todo: optimize/remove this
		float& operator[](size_t index);

		// todo: optimize/remove this
		const float& operator[](size_t index) const;
	};

	vec2_t operator+(vec2_t lhs, const vec2_t& rhs);
	vec2_t operator-(vec2_t lhs, const vec2_t& rhs);
	vec2_t operator*(vec2_t lhs, float rhs);
	vec2_t operator*(float lhs, vec2_t rhs);
} // namespace aech::math
