#pragma once

#include "vec.hpp"

namespace aech::math
{
	// two component vector
	template<typename ScalarType>
	struct vec3_with_aliases_t : vec_t<ScalarType, 3>
	{
		ScalarType& x{ vec_t<ScalarType, 3>::data[0] };
		ScalarType& y{ vec_t<ScalarType, 3>::data[1] };
		ScalarType& z{ vec_t<ScalarType, 3>::data[2] };

		using vec_t<ScalarType, 3>::vec_t;
		using vec_t<ScalarType, 3>::operator[];

		vec3_with_aliases_t(const vec3_with_aliases_t& vec3)
		{
			vec_t<ScalarType, 3>::data = vec3.data;
		}

		vec3_with_aliases_t& operator+=(const vec3_with_aliases_t& rhs)
		{
			vec_t<ScalarType, 3>::operator+=(rhs);
			return *this;
		}

		vec3_with_aliases_t& operator-=(const vec3_with_aliases_t& rhs)
		{
			vec_t<ScalarType, 3>::operator-=(rhs);
			return *this;
		}

		vec3_with_aliases_t& operator*=(ScalarType rhs)
		{
			vec_t<ScalarType, 3>::operator*=(rhs);
			return *this;
		}

		vec3_with_aliases_t& operator/=(ScalarType rhs)
		{
			vec_t<ScalarType, 3>::operator/=(rhs);
			return *this;
		}

		vec3_with_aliases_t& operator=(const vec3_with_aliases_t& vec)
		{
			vec_t<ScalarType, 3>::operator=(vec);
			return *this;
		}
	};

	using vec3_t = vec3_with_aliases_t<float>;

	template<typename ScalarType>
	vec3_with_aliases_t<ScalarType> operator+(vec3_with_aliases_t<ScalarType> lhs, const vec3_with_aliases_t<ScalarType>& rhs)
	{
		return lhs += rhs;
	}

	template<typename ScalarType>
	vec3_with_aliases_t<ScalarType> operator-(vec3_with_aliases_t<ScalarType> lhs, const vec3_with_aliases_t<ScalarType>& rhs)
	{
		return lhs -= rhs;
	}

	template<typename ScalarType>
	vec3_with_aliases_t<ScalarType> operator*(vec3_with_aliases_t<ScalarType> lhs, ScalarType rhs)
	{
		return lhs *= rhs;
	}

	template<typename ScalarType>
	vec3_with_aliases_t<ScalarType> operator/(vec3_with_aliases_t<ScalarType> lhs, ScalarType rhs)
	{
		return lhs /= rhs;
	}

	template<typename ScalarType>
	vec3_with_aliases_t<ScalarType> operator*(ScalarType lhs, vec3_with_aliases_t<ScalarType> rhs)
	{
		return rhs *= lhs;
	}
} // namespace aech::math
