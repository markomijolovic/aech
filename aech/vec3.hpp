#pragma once
#include "vec4.hpp"

namespace aech::math
{
	// three component vector
	class vec3_t
	{
	public:
		float x{};
		float y{};
		float z{};

		// initialize all components to zero
		vec3_t() = default;

		// initialize all components from the first three
		// components of the given four component vector
		explicit vec3_t(const vec4_t& vec4);

		// initialize from three scalars
		vec3_t(float x, float y, float z);

		vec3_t& operator+=(const vec3_t& vec3);
		vec3_t& operator-=(const vec3_t& vec3);
		vec3_t& operator*=(float rhs);
		vec3_t& operator/=(float rhs);

		// todo: optimize/remove this
		float& operator[](size_t index);

		// todo: optimize/remove this
		const float& operator[](size_t index) const;
	};

	vec3_t operator+(vec3_t lhs, const vec3_t& rhs);
	vec3_t operator-(vec3_t lhs, const vec3_t& rhs);
	vec3_t operator*(vec3_t lhs, float rhs);
	vec3_t operator*(float lhs, vec3_t rhs);
	vec3_t operator/(vec3_t lhs, float rhs);
} // namespace aech::math
