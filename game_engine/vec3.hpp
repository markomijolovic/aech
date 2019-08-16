#pragma once

namespace aech
{
	class vec3_t
	{
	public:
		float x, y, z;

		vec3_t() = default;
		vec3_t(float x, float y, float z);

		vec3_t& operator+=(const vec3_t& vec3);
		vec3_t& operator-=(const vec3_t& vec3);
		vec3_t& operator*=(const vec3_t& vec3);
		vec3_t& operator*=(float rhs);
	};

	vec3_t operator+(vec3_t lhs, const vec3_t& rhs);
	vec3_t operator-(vec3_t lhs, const vec3_t& rhs);
	vec3_t operator*(vec3_t lhs, const vec3_t& rhs);
	vec3_t operator*(vec3_t lhs, float rhs);
}