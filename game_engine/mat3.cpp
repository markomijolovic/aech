#include "mat3.hpp"

namespace aech::math
{
	mat3_t mat3_t::operator*(const mat3_t& rhs) const
	{
		mat3_t retval{};

		for (auto row = 0; row < 3; row++)
		{
			for (auto col = 0; col < 3; col++)
			{
				auto sum = 0.0f;
				for (auto k = 0; k < 3; k++)
				{
					sum += data[row][k] * rhs.data[k][col];
				}
				retval.data[row][col] = sum;
			}
		}

		return retval;
	}

	mat3_t mat3_t::operator*(float rhs) const
	{
		mat3_t retval{ *this };

		for (auto row = 0; row < 3; row++)
		{
			for (auto col = 0; col < 3; col++)
			{
				retval.data[row][col] *= rhs;
			}
		}

		return retval;
	}

	mat3_t operator*(float lhs, const mat3_t& rhs)
	{
		return rhs * lhs;
	}


	mat3_t mat3_t::operator*=(const mat3_t& rhs)
	{
		mat3_t retval{*this};

		return *this = retval * rhs;
	}
}
