#include "mat4.hpp"
#include <assert.h>

namespace aech::math
{
	mat4_t mat4_t::operator*(const mat4_t& rhs) const
	{
		mat4_t result{};

		for (auto row = 0; row < 4; row++)
		{
			for (auto col = 0; col < 4; col++)
			{
				auto sum = 0.0f;
				for (auto i = 0; i < 4; i++)
				{
					sum += data[row][i] * rhs.data[i][col];
				}
				result.data[row][col] = sum;
			}
		}

		return result;
	}

	vec4_t mat4_t::operator*(const vec4_t& rhs) const
	{
		return {
			(rhs.x * data[0][0]) + (rhs.y * data[0][1]) + (rhs.z * data[0][2]) + (rhs.w * data[0][3]),
			(rhs.x * data[1][0]) + (rhs.y * data[1][1]) + (rhs.z * data[1][2]) + (rhs.w * data[1][3]),
			(rhs.x * data[2][0]) + (rhs.y * data[2][1]) + (rhs.z * data[2][2]) + (rhs.w * data[2][3]),
			(rhs.x * data[3][0]) + (rhs.y * data[3][1]) + (rhs.z * data[3][2]) + (rhs.w * data[3][3])
		};
	}


	mat4_t& mat4_t::operator*=(const mat4_t& rhs)
	{
		mat4_t result{*this};
		return *this = (result * rhs);
	}

	mat4_t mat4_t::operator*(float rhs) const
	{
		mat4_t result{ *this };
		for (auto row = 0; row < 4; row++)
		{
			for (auto col = 0; col < 4; col++)
			{
				result.data[row][col] *= rhs;
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

		size_t i{}, j{};
		for (auto el: list)
		{
			data[i][j] = el;
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
		for (const auto &vec4: list)
		{
			data[i][0] = vec4.x;
			data[i][1] = vec4.y;
			data[i][2] = vec4.z;
			data[i][3] = vec4.w;
			i++;
		}
	}
}
