#include "transform.hpp"
#include "mat4.hpp"
#include "transforms.hpp"

aech::mat4_t aech::transform_t::get_transform_matrix() const
{
	auto transform_matrix = rotate(rotation.x, { 1.0f, 0.0f, 0.0f });
	transform_matrix *= rotate(rotation.y, { 0.0f, 1.0f, 0.0f });
	transform_matrix *= rotate(rotation.z, { 0.0f, 0.0f, 1.0f });

	transform_matrix *= aech::scale(scale);
	transform_matrix *= translate(position);

	return transform_matrix;
}


aech::vec3_t aech::transform_t::get_forward_vector() const
{
	vec4_t forward{ 1, 0, 0 , 0};
	forward = rotate(radians(rotation.x), { 1, 0, 0 }) * vec4_t { 0, 0, -1, 0 };
	forward = rotate(radians(rotation.y), { 0, 1, 0 }) * forward;
	forward = rotate(radians(rotation.z), { 0, 0, 1 }) * forward;
	return vec3_t{ forward };
}
