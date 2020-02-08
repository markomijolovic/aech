#pragma once

namespace aech::math
{
	class vec3_t;

	class vec4_t
	{
	public:
		float x{};
		float y{};
		float z{};
		float w{};

		vec4_t() = default;
		vec4_t(const vec3_t& vec3, float w);
		vec4_t(float x, float y, float z, float w);

		float& operator[](size_t index);
		const float& operator[](size_t index) const;

		vec4_t& operator+=(const vec4_t& vec4);
		vec4_t& operator-=(const vec4_t& vec4);
		vec4_t& operator*=(float rhs);
		vec4_t& operator/=(float rhs);
	};

	vec4_t operator*(vec4_t lhs, float rhs);
	vec4_t operator*(float lhs, vec4_t rhs);
	vec4_t operator+(vec4_t lhs, const vec4_t& rhs);
	vec4_t operator-(vec4_t lhs, const vec4_t& rhs);
	vec4_t operator/(vec4_t lhs, float rhs);
	vec4_t operator/(float lhs, vec4_t rhs);
} // namespace aech::math