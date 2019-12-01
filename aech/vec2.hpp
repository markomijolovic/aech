#pragma once

namespace aech::math
{
	class vec2_t
	{
	public:
		float x{};
		float y{};

		vec2_t() = default;
		vec2_t(float x, float y);

		vec2_t& operator+=(const vec2_t& vec2);
		vec2_t& operator-=(const vec2_t& vec2);
		vec2_t& operator*=(float rhs);

		float& operator[](size_t index);
		const float &operator[](size_t index) const;
	};

	vec2_t operator+(vec2_t lhs, const vec2_t& rhs);
	vec2_t operator-(vec2_t lhs, const vec2_t& rhs);
	vec2_t operator*(vec2_t lhs, float rhs);
	vec2_t operator*(float lhs, vec2_t rhs);
} // namespace aech::math
