#pragma once

namespace aech::math
{
	class vec3_t;

	// four component vector
	class vec4_t
	{
	public:
		float x{};
		float y{};
		float z{};
		float w{};

		// initializes all components to 0
		vec4_t() = default;

		// initialize first three components from the given three component vector
		// initialize the last component with the given scalar
		vec4_t(const vec3_t& vec3, float w);

		// initialize from four scalars
		vec4_t(float x, float y, float z, float w);

		// todo: optimize/remove this
		float& operator[](size_t index);

		// todo: optimize/remove this
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
