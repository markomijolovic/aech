#include "mat4.hpp"

namespace aech
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
		return vec4_t{
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


}
