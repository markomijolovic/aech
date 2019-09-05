#pragma once

namespace aech::math
{
	class mat3_t
	{
	public:
		float data[3][3] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		};

		mat3_t operator*(const mat3_t& rhs) const;
		mat3_t operator*(float rhs) const;
		mat3_t operator*=(const mat3_t& rhs);
	};

	mat3_t operator*(float lhs, const mat3_t& rhs);
}
