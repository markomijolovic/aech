#pragma once

#include "vec.hpp"

namespace aech::math {
// returns the dot product of the given two vectors
template <typename ScalarType, size_t Dimension>
auto dot(const vec_t<ScalarType, Dimension>& a, const vec_t<ScalarType, Dimension>& b)
{
    auto ret = ScalarType {};

    for (size_t i = 0; i < a.data.size(); i++) {
        ret += a[i] * b[i];
    }

    return ret;
}

// returns the squared distance between the given two vectors
template <typename ScalarType, size_t Dimension>
auto distance(const vec_t<ScalarType, Dimension>& a, const vec_t<ScalarType, Dimension>& b)
{
    ScalarType retval {};
    for (size_t i = 0; i < a.data.size(); i++) {
        retval += std::pow((a.data[i] - b.data[i]), 2);
    }
    return std::sqrt(retval);
}

// returns the dot product of two vectors with three components
template <typename ScalarType>
auto cross_product(const vec_t<ScalarType, 3>& a, const vec_t<ScalarType, 3>& b)
{
    return vec_t<ScalarType, 3> {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    };
}
}
