#include "mat4.hpp"

#include <cassert>

namespace aech::math
{
	mat4_t mat4_t::operator*(const mat4_t& rhs) const
	{
		mat4_t result{};

		for (auto row = 0; row < 4; row++)
		{
			for (auto col = 0; col < 4; col++)
			{
				auto sum = 0.0F;
				for (auto i = 0; i < 4; i++)
				{
					sum += m_data[row][i] * rhs.m_data[i][col];
				}
				result.m_data[row][col] = sum;
			}
		}

		return result;
	}

	vec4_t mat4_t::operator*(const vec4_t& rhs) const
	{
		return
		{
			rhs.x * m_data[0][0] + rhs.y * m_data[0][1] + rhs.z * m_data[0][2] + rhs.w * m_data[0][3],
			rhs.x * m_data[1][0] + rhs.y * m_data[1][1] + rhs.z * m_data[1][2] + rhs.w * m_data[1][3],
			rhs.x * m_data[2][0] + rhs.y * m_data[2][1] + rhs.z * m_data[2][2] + rhs.w * m_data[2][3],
			rhs.x * m_data[3][0] + rhs.y * m_data[3][1] + rhs.z * m_data[3][2] + rhs.w * m_data[3][3]
		};
	}


	mat4_t& mat4_t::operator*=(const mat4_t& rhs)
	{
		const auto result{*this};
		return *this = result * rhs;
	}

	mat4_t mat4_t::operator*(float rhs) const
	{
		auto result{*this};
		for (auto row = 0; row < 4; row++)
		{
			for (auto col = 0; col < 4; col++)
			{
				result.m_data[row][col] *= rhs;
			}
		}
		return result;
	}

	mat4_t operator*(float lhs, const mat4_t& rhs)
	{
		return rhs * lhs;
	}


	mat4_t::mat4_t(const std::initializer_list<float>& list)
	{
		assert(list.size() == 16);

		size_t i{};
		size_t j{};
		for (auto el : list)
		{
			m_data[i][j] = el;
			if (j++ == 3)
			{
				i++;
				j = 0;
			}
		}
	}


	mat4_t::mat4_t(const std::initializer_list<vec4_t>& list)
	{
		assert(list.size() == 4);

		size_t i{};
		for (const auto& vec4 : list)
		{
			m_data[i][0] = vec4.x;
			m_data[i][1] = vec4.y;
			m_data[i][2] = vec4.z;
			m_data[i][3] = vec4.w;
			i++;
		}
	}

	vec4_t& mat4_t::operator[](size_t i)
	{
		return m_data[i];
	}

	const vec4_t& mat4_t::operator[](size_t i) const
	{
		return m_data[i];
	}
} // namespace aech::math
