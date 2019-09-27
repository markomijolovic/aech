#include "camera.hpp"
#include "main.hpp"


void aech::camera_t::rotate_x(float amount)
{
	m_transform->rotation.x += amount;
	m_transform->rotation.x = std::clamp(m_transform->rotation.x, -89.0F, 89.0F);
	m_dirty = true;
}

const aech::camera_frustum_t& aech::camera_t::frustum()
{
	if (m_dirty)
	{
		recalculate_frustum();
		m_dirty = false;
	}
	return m_frustum;
}


aech::camera_t::camera_t(const math::mat4_t& projection, transform_t* transform, camera_frustum_t frustum)
	: m_projection{projection}, m_transform{transform}, m_frustum{frustum}
{
}


aech::math::mat4_t aech::camera_t::projection() const
{
	return m_projection;
}


aech::transform_t* aech::camera_t::transform() const
{
	return m_transform;
}

aech::math::mat4_t aech::camera_t::view_matrix() const
{
	return math::get_view_matrix(*m_transform);
}

bool aech::camera_t::sees(const graphics::mesh_t& mesh)
{
	return frustum().intersects(mesh.bounding_box());
}


bool aech::camera_t::sees(const math::vec3_t& centre, float radius)
{
	return frustum().intersects(centre, radius);
}

void aech::camera_t::translate(const math::vec3_t& amount)
{
	m_transform->position += amount;
	m_dirty = true;
}

void aech::camera_t::rotate_y(float amount)
{
	m_dirty = true;
	m_transform->rotation.y += amount;
	m_transform->rotation.y = std::fmod(m_transform->rotation.y, 360.0F);
}

void aech::camera_t::recalculate_frustum()
{
	auto pv = m_projection * math::get_view_matrix(*m_transform);
	m_frustum.recalculate(pv);
}
