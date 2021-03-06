#pragma once

#include "vec.hpp"

#include <cmath>

namespace aech::math {
// returns the dot product of the given two vectors
template <typename ScalarType, std::size_t Dimension>
auto dot(const vec_t<ScalarType, Dimension> &a, const vec_t<ScalarType, Dimension> &b) noexcept
{
    auto ret = ScalarType{};

    for (std::size_t i = 0; i < a.data.size(); i++) {
        ret += a[i] * b[i];
    }

    return ret;
}

// returns the distance between the given two vectors
template <typename ScalarType, std::size_t Dimension>
auto distance(const vec_t<ScalarType, Dimension> &a, const vec_t<ScalarType, Dimension> &b) noexcept
{
    ScalarType retval{};
    for (std::size_t i = 0; i < a.data.size(); i++) {
        retval += std::pow((a.data[i] - b.data[i]), 2);
    }
    return std::sqrt(retval);
}

// returns the cross product of two 3D vectors
template <typename ScalarType>
auto cross_product(const vec_t<ScalarType, 3> &a, const vec_t<ScalarType, 3> &b) noexcept
{
    return vec_t<ScalarType, 3>{
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]};
}
} // namespace aech::math // namespace aech::math
