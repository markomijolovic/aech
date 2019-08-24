#include "transforms.hpp"
#include "types.hpp"
#include "mat3.hpp"
#include "transform.hpp"

namespace aech
{
	mat4_t translate(float x, float y, float z)
	{
		mat4_t retval{};
		retval.data[0][3] = x;
		retval.data[1][3] = y;
		retval.data[2][3] = z;
		return retval;
	}

	mat4_t scale(float x, float y, float z)
	{
		mat4_t retval{};
		retval.data[0][0] *= x;
		retval.data[1][1] *= y;
		retval.data[2][2] *= z;
		return retval;
	}

	float length(const vec3_t& axis)
	{
		return sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	}

	vec3_t normalize(const vec3_t& axis)
	{
		return axis * (1 / length(axis));
	}

	mat4_t rotate(float angle, const vec3_t& axis_)
	{
		auto axis = normalize(axis_);
		float s = sin(angle);
		float c = cos(angle);
		float oc = 1.0 - c;

		return mat4_t{
			oc* axis.x* axis.x + c,           oc* axis.x* axis.y - axis.z * s,  oc* axis.z* axis.x + axis.y * s,  0.0,
			oc* axis.x* axis.y + axis.z * s,  oc* axis.y* axis.y + c,           oc* axis.y* axis.z - axis.x * s,  0.0,
			oc* axis.z* axis.x - axis.y * s,  oc* axis.y* axis.z + axis.x * s,  oc* axis.z* axis.z + c,           0.0,
			0.0,                                0.0,                                0.0,                                1.0
		};
	}

	mat4_t get_view_matrix(const transform_t& transform)
	{
		mat4_t mat_pitch{};
		mat4_t mat_yaw{};
		mat_pitch = rotate(radians(-transform.rotation.x), { 1,0,0 });
		mat_yaw = rotate(radians(-transform.rotation.y), { 0,1,0 });
		auto rotate = mat_pitch * mat_yaw;
		auto trans = translate(-transform.position.x, -transform.position.y, -transform.position.z);
		return rotate * trans;
	}


	mat4_t translate(const vec3_t& vec3)
	{
		return translate(vec3.x, vec3.y, vec3.z);
	}

	mat4_t scale(const vec3_t& vec3)
	{
		return scale(vec3.x, vec3.y, vec3.z);
	}
}
