#include "transforms.hpp"
#include "aech_types.hpp"
#include "transform.hpp"

namespace aech::math
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

	mat4_t get_view_matrix(const aech::transform_t& transform)
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


	mat4_t look_at(const vec3_t& eye, const vec3_t& centre, const vec3_t& up)
	{
		auto w = normalize(eye - centre);
		auto u = normalize(cross(up, w));
		auto v = normalize(cross(w, u));
		auto first_mat = mat4_t{ u.x, u.y, u.z, 0, v.x, v.y, v.z, 0, w.x, w.y, w.z, 0, 0, 0, 0, 1 };
		auto second_mat = mat4_t{1, 0, 0, -eye.x,
								0, 1, 0, -eye.y, 
								0, 0, 1, -eye.z,
								0, 0, 0, 1};
		return first_mat * second_mat;
	}

	mat4_t perspective(float fov_y, float aspect, float z_near, float z_far)
	{
		auto theta = radians(fov_y / 2);
		auto d = cos(theta) / sin(theta);
		auto a = -(z_far + z_near) / (z_far - z_near);
		auto b = -(2 * z_far * z_near) / (z_far - z_near);
		return mat4_t
		{
			static_cast<float>(d / aspect), 0, 0, 0,
			0,
			static_cast<float>(d), 0, 0,
			0, 0, a, b,
			0, 0, -1, 0
		};
	}


	vec3_t cross(const vec3_t& first, const vec3_t& second)
	{
		return vec3_t
		{
			first.y * second.z - first.z * second.y,
			-(first.x * second.z - first.z * second.x),
			first.x* second.y - first.y * second.x
		};
	}


	mat4_t orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		return mat4_t
		{
			{2/(right-left), 0, 0, -(right+left)/(right-left)},
			{0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom)},
			{0, 0, -2/(far-near), -(far +near)/(far-near)},
			{0, 0, 0, 1}
		};
	}
}
