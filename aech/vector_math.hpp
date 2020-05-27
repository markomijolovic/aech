#pragma once

#include <numeric>

#include "vec.hpp"

namespace aech::math
{
	// returns the dot product of the given two vectors
	template<typename ScalarType, size_t Dimension>
	auto dot(const vec_t<ScalarType, Dimension> &a, const vec_t<ScalarType, Dimension>& b)
	{
		return std::accumulate(a.data.begin(), a.data.end(), ScalarType{ });
	}
	
	// returns the squared distance between the given two vectors
	template<typename ScalarType, size_t Dimension>
	auto distance(const vec_t<ScalarType, Dimension>& a, const vec_t<ScalarType, Dimension>& b)
	{
		ScalarType retval{};
		for (size_t i = 0; i < a.data.size(); i ++)
		{
			retval += std::pow((a.data[i] - b.data[i]), 2);
		}
		return std::sqrt(retval);
	}
}
