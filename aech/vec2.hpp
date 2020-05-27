#pragma once

#include "vec.hpp"

namespace aech::math
{
	// two component vector
	template<typename ScalarType>
	struct vec2_with_aliases_t : vec_t<ScalarType, 2>
	{
		ScalarType& x{ vec_t<ScalarType, 2>::data[0] };
		ScalarType& y{ vec_t<ScalarType, 2>::data[1] };

		using vec_t<ScalarType, 2>::vec_t;
		using vec_t<ScalarType, 2>::operator[];

		vec2_with_aliases_t(const vec2_with_aliases_t& vec3)
		{
			vec_t<ScalarType, 2>::data = vec3.data;
		}

		vec2_with_aliases_t& operator+=(const vec2_with_aliases_t& rhs)
		{
			vec_t<ScalarType, 2>::operator+=(rhs);
			return *this;
		}

		vec2_with_aliases_t& operator-=(const vec2_with_aliases_t& rhs)
		{
			vec_t<ScalarType, 2>::operator-=(rhs);
			return *this;
		}

		vec2_with_aliases_t& operator*=(ScalarType rhs)
		{
			vec_t<ScalarType, 2>::operator*=(rhs);
			return *this;
		}

		vec2_with_aliases_t& operator/=(ScalarType rhs)
		{
			vec_t<ScalarType, 2>::operator/=(rhs);
			return *this;
		}

		vec2_with_aliases_t& operator= (const vec2_with_aliases_t& vec)
		{
			vec_t<ScalarType, 2>::operator=(vec);
			return *this;
		}
	};

	using vec2_t = vec2_with_aliases_t<float>;

	template<typename ScalarType>
	vec2_with_aliases_t<ScalarType> operator+(vec2_with_aliases_t<ScalarType> lhs, const vec2_with_aliases_t<ScalarType>& rhs)
	{
		return lhs += rhs;
	}

	template<typename ScalarType>
	vec2_with_aliases_t<ScalarType> operator-(vec2_with_aliases_t<ScalarType> lhs, const vec2_with_aliases_t<ScalarType>& rhs)
	{
		return lhs -= rhs;
	}

	template<typename ScalarType>
	vec2_with_aliases_t<ScalarType> operator*(vec2_with_aliases_t<ScalarType> lhs, ScalarType rhs)
	{
		return lhs *= rhs;
	}

	template<typename ScalarType>
	vec2_with_aliases_t<ScalarType> operator/(vec2_with_aliases_t<ScalarType> lhs, ScalarType rhs)
	{
		return lhs /= rhs;
	}

	template<typename ScalarType>
	vec2_with_aliases_t<ScalarType> operator*(ScalarType lhs, vec2_with_aliases_t<ScalarType> rhs)
	{
		return rhs *= lhs;
	}
} // namespace aech::math
