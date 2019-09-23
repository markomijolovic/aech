#pragma once
#include "mat4.hpp"
#include "camera_frustum.hpp"
#include "transform.hpp"
#include "mesh.hpp"

namespace aech
{
	class camera_t
	{
	public:
		camera_t() = default;
		camera_t(const math::mat4_t& projection, transform_t* transform, camera_frustum_t frustum = {});
		const camera_frustum_t& frustum();
		void                    rotate_x(float amount);
		void                    rotate_y(float amount);
		void                    translate(const math::vec3_t& amount);
		// does this camera "see" this mesh?
		bool                       sees(const graphics::mesh_t& mesh);
		bool                       sees(const math::vec3_t&centre, float radius);
		[[nodiscard]] math::mat4_t view_matrix() const;
		[[nodiscard]] math::mat4_t projection() const;
	private:
		math::mat4_t     m_projection{};
		camera_frustum_t m_frustum{};
		transform_t*     m_transform{};
		bool             m_dirty{true};
		void             recalculate_frustum();
	};
} // namespace aech
