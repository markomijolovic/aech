#pragma once

#include "vec.hpp"
#include "vector_math.hpp"

#include <initializer_list>

namespace aech::math {
// square matrix
template <typename ScalarType, std::size_t Dimension>
class mat_t {
public:
    // initialize to identity matrix
    mat_t() noexcept
    {
        for (std::size_t i = 0; i < Dimension; i++) {
            m_data[i][i] = ScalarType{1};
        }
    }

    // initialize from init_list of ScalarType
    mat_t(std::initializer_list<ScalarType> init_list)
    {
        assert(init_list.size() == Dimension * Dimension);

        std::size_t i{};
        for (std::size_t j{}; auto el: init_list) {
            m_data[i][j] = el;
            if (j++ == Dimension - 1) {
                i++;
                j = 0;
            }
        }
    }

    // initialize from init_list of vec_ts
    mat_t(std::initializer_list<vec_t<ScalarType, Dimension>> init_list)
    {
        assert(init_list.size() == Dimension);

        for (std::size_t i{}; const auto &el: init_list) {
            m_data[i++] = el;
        }
    }

    auto operator*=(const mat_t &rhs) -> mat_t &
    {
        const auto result{*this};

        return *this = result * rhs;
    }

    [[nodiscard]] auto operator*(const mat_t &rhs) const -> mat_t
    {
        mat_t<ScalarType, Dimension> result{};

        for (std::size_t i = 0; i < Dimension; i++) {
            for (std::size_t j = 0; j < Dimension; j++) {
                ScalarType res{};

                for (std::size_t k = 0; k < Dimension; k++) {
                    res += m_data[i][k] * rhs.m_data[k][j];
                }

                result.m_data[i][j] = res;
            }
        }

        return result;
    }

    [[nodiscard]] auto operator*(const vec_t<ScalarType, Dimension> &rhs) const -> vec_t<ScalarType, Dimension>
    {
        vec_t<ScalarType, Dimension> result{};

        for (std::size_t i = 0; i < result.data.size(); i++) {
            result[i] = dot(m_data[i], rhs);
        }

        return result;
    }

    [[nodiscard]] auto operator*(ScalarType rhs) const -> mat_t
    {
        mat_t<ScalarType, Dimension> result{};

        for (std::size_t row = 0; row < Dimension; row++) {
            for (std::size_t col = 0; col < Dimension; col++) {
                result.m_data[row][col] = m_data[row][col] * rhs;
            }
        }

        return result;
    }

    auto operator[](std::size_t i) const -> const vec_t<ScalarType, Dimension> &
    {
        return m_data[i];
    }

    auto operator[](std::size_t i) -> vec_t<ScalarType, Dimension> &
    {
        return m_data[i];
    }

private:
    std::array<vec_t<ScalarType, Dimension>, Dimension> m_data{};
};

template <typename ScalarType, std::size_t Dimension>
auto operator*(float lhs, const mat_t<ScalarType, Dimension> &rhs) -> mat_t<ScalarType, Dimension>
{
    return rhs * lhs;
}

using mat4_t = mat_t<float, 4>;
} // namespace aech::math
