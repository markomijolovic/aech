#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

namespace aech::math {
// hide storage details
namespace detail {
template <typename ScalarType, std::size_t Dimension>
struct vec_t_storage {
    // default storage (no aliases)
    std::array<ScalarType, Dimension> data{ScalarType{}};
};

template <typename ScalarType>
struct vec_t_storage<ScalarType, 2> {
    union {
        std::array<ScalarType, 2> data{ScalarType{}};

        // this is actually not standard-compliant, but it works lmao
        struct {
            ScalarType x;
            ScalarType y;
        };
    };
};

template <typename ScalarType>
struct vec_t_storage<ScalarType, 3> {
    union {
        std::array<ScalarType, 3> data{ScalarType{}};

        // this is actually not standard-compliant, but it works lmao
        struct {
            ScalarType x;
            ScalarType y;
            ScalarType z;
        };
    };
};

template <typename ScalarType>
struct vec_t_storage<ScalarType, 4> {
    union {
        std::array<ScalarType, 4> data{ScalarType{}};

        // this is actually not standard-compliant, but it works lmao
        struct {
            ScalarType x;
            ScalarType y;
            ScalarType z;
            ScalarType w;
        };
    };
};
} // namespace // namespace

template <typename ScalarType, std::size_t Dimension>
struct vec_t : detail::vec_t_storage<ScalarType, Dimension> {
    using detail::vec_t_storage<ScalarType, Dimension>::data;

    vec_t() = default;

    // initialize all components to scalar
    template <typename... Scalar>
    vec_t(Scalar... args) requires(sizeof...(Scalar) >= 2)
    {
        auto init = {args...};

        std::copy(init.begin(), init.end(), data.begin());
    }

    explicit vec_t(ScalarType scalar)
    {
        std::fill(data.begin(), data.end(), scalar);
    }

    // initialize from init list containing <= number of elements of vector
    vec_t(std::initializer_list<ScalarType> init_list)
    {
        assert(init_list.size() <= Dimension);

        std::copy(init_list.begin(), init_list.end(), data.begin());
    }

    // initialize from a vector (with OtherDimension == Dimension - 1) and a single scalar
    template <std::size_t OtherDimension>
    vec_t(const vec_t<ScalarType, OtherDimension> &vec, ScalarType scalar)
    {
        assert(OtherDimension == Dimension - 1);

        std::copy(vec.data.begin(), vec.data.end(), data.begin());
        std::fill(data.begin(), data.end(), scalar);
    }

    // initialize from a bigger vector (take as many elements as you need)
    template <std::size_t OtherDimension>
    requires(OtherDimension > Dimension) explicit vec_t(const vec_t<ScalarType, OtherDimension> &vec)
    {
        assert(OtherDimension > Dimension);

        std::copy_n(vec.data.begin(), Dimension, data.begin());
    }

    // initialize from another vector and init list
    template <std::size_t OtherDimension>
    vec_t(const vec_t<ScalarType, OtherDimension> &vec, std::initializer_list<ScalarType> init_list)
    {
        assert(OtherDimension + init_list.size() == Dimension);

        auto it = std::copy(vec.data.begin(), vec.data.end(), data.begin());
        std::copy(init_list.begin(), init_list.end(), it);
    }

    auto operator+=(const vec_t &rhs) -> vec_t &
    {
        for (std::size_t i = 0; i < Dimension; i++) {
            data[i] += rhs.data[i];
        }

        return *this;
    }

    auto operator-=(const vec_t &rhs) -> vec_t &
    {
        for (std::size_t i = 0; i < Dimension; i++) {
            data[i] -= rhs.data[i];
        }

        return *this;
    }

    auto operator*=(float rhs) -> vec_t &
    {
        for (auto &el: data) {
            el *= rhs;
        }

        return *this;
    }

    auto operator/=(float rhs) -> vec_t &
    {
        for (auto &el: data) {
            el /= rhs;
        }

        return *this;
    }

    auto operator[](std::size_t index) -> ScalarType &
    {
        assert(index < Dimension);

        return data[index];
    }

    auto operator[](std::size_t index) const -> const ScalarType &
    {
        assert(index < Dimension);

        return data[index];
    }
};

template <std::size_t Dimension, typename ScalarType>
auto operator+(vec_t<ScalarType, Dimension> lhs, const vec_t<ScalarType, Dimension> &rhs) -> vec_t<ScalarType, Dimension>
{
    return lhs += rhs;
}

template <std::size_t Dimension, typename ScalarType>
auto operator-(vec_t<ScalarType, Dimension> lhs, const vec_t<ScalarType, Dimension> &rhs) -> vec_t<ScalarType, Dimension>
{
    return lhs -= rhs;
}

template <std::size_t Dimension, typename ScalarType>
auto operator*(vec_t<ScalarType, Dimension> lhs, ScalarType rhs) -> vec_t<ScalarType, Dimension>
{
    return lhs *= rhs;
}

template <std::size_t Dimension, typename ScalarType>
auto operator/(vec_t<ScalarType, Dimension> lhs, ScalarType rhs) -> vec_t<ScalarType, Dimension>
{
    return lhs /= rhs;
}

template <std::size_t Dimension, typename ScalarType>
auto operator*(ScalarType lhs, vec_t<ScalarType, Dimension> rhs) -> vec_t<ScalarType, Dimension>
{
    return rhs *= lhs;
}

// deduction guide for variadic constructor
template <typename... Scalar>
vec_t(Scalar... args) -> vec_t<std::common_type_t<Scalar...>, sizeof...(args)>;

// standard specializations
using vec2_t = vec_t<float, 2>;
using vec3_t = vec_t<float, 3>;
using vec4_t = vec_t<float, 4>;
} // namespace aech::math
