#pragma once
#include "texture_cube.hpp"
#include "vec3.hpp"

namespace aech::graphics
{
	class light_probe_t
	{
	public:
		light_probe_t(math::vec3_t m_position, float m_radius);

		[[nodiscard]] texture_cube_t* irradiance() const;
		[[nodiscard]] texture_cube_t* prefiltered() const;
		[[nodiscard]] math::vec3_t    position() const;
		[[nodiscard]] float           radius() const;


		void set_irradiance(texture_cube_t* irradiance);
		void set_prefiltered(texture_cube_t* prefiltered);
	private:
		texture_cube_t* m_irradiance{};
		texture_cube_t* m_prefiltered{};
		math::vec3_t    m_position{};
		float           m_radius{};
	};
} // namespace aech::graphics
