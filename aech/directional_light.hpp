#pragma once
#include "transform.hpp"

#include "vec3.hpp"


namespace aech::graphics
{
	class directional_light_t
	{
	public:
		directional_light_t() = default;
		directional_light_t(math::vec3_t colour, float intensity, transform_t* transform);

		[[nodiscard]] math::vec3_t colour() const;
		[[nodiscard]] float        intensity() const;
		[[nodiscard]] transform_t* transform() const;

	private:
		math::vec3_t m_colour{1.0F, 1.0F, 1.0F};
		float        m_intensity = 1.0F;
		transform_t* m_transform{};
	};
} // namespace aech::graphics
