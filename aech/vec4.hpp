#pragma once

#include "vec.hpp"

namespace aech::math
{
	// two component vector
	template<typename ScalarType>
	struct vec4_with_aliases_t : vec_t<ScalarType, 4>
	{
		ScalarType& x{ vec_t<ScalarType, 4>::data[0] };
		ScalarType& y{ vec_t<ScalarType, 4>::data[1] };
		ScalarType& z{ vec_t<ScalarType, 4>::data[2] };
		ScalarType& w{ vec_t<ScalarType, 4>::data[3] };

		using vec_t<ScalarType, 4>::vec_t;
		using vec_t<ScalarType, 4>::operator[];

		vec4_with_aliases_t(const vec4_with_aliases_t& vec3)
		{
			vec_t<ScalarType, 4>::data = vec3.data;
		}

		vec4_with_aliases_t& operator+=(const vec4_with_aliases_t& rhs)
		{
			vec_t<ScalarType, 4>::operator+=(rhs);
			return *this;
		}

		vec4_with_aliases_t& operator-=(const vec4_with_aliases_t& rhs)
		{
			vec_t<ScalarType, 4>::operator-=(rhs);
			return *this;
		}

		vec4_with_aliases_t& operator*=(ScalarType rhs)
		{
			vec_t<ScalarType, 4>::operator*=(rhs);
			return *this;
		}

		vec4_with_aliases_t& operator/=(ScalarType rhs)
		{
			vec_t<ScalarType, 4>::operator/=(rhs);
			return *this;
		}

		vec4_with_aliases_t& operator=(const vec4_with_aliases_t& vec)
		{
			vec_t<ScalarType, 4>::operator=(vec);
			return *this;
		}
	};

	using vec4_t = vec4_with_aliases_t<float>;

	template<typename ScalarType>
	vec4_with_aliases_t<ScalarType> operator+(vec4_with_aliases_t<ScalarType> lhs, const vec4_with_aliases_t<ScalarType>& rhs)
	{
		return lhs += rhs;
	}

	template<typename ScalarType>
	vec4_with_aliases_t<ScalarType> operator-(vec4_with_aliases_t<ScalarType> lhs, const vec4_with_aliases_t<ScalarType>& rhs)
	{
		return lhs -= rhs;
	}

	template<typename ScalarType>
	vec4_with_aliases_t<ScalarType> operator*(vec4_with_aliases_t<ScalarType> lhs, ScalarType rhs)
	{
		return lhs *= rhs;
	}

	template<typename ScalarType>
	vec4_with_aliases_t<ScalarType> operator/(vec4_with_aliases_t<ScalarType> lhs, ScalarType rhs)
	{
		return lhs /= rhs;
	}

	template<typename ScalarType>
	vec4_with_aliases_t<ScalarType> operator*(ScalarType lhs, vec4_with_aliases_t<ScalarType> rhs)
	{
		return rhs *= lhs;
	}
} // namespace aech::math
