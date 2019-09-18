#pragma once
#include "texture_cube.hpp"
#include "vec3.hpp"

namespace aech::graphics
{
	class light_probe_t
	{
	public:
		light_probe_t(math::vec3_t m_position, float m_radius);

		texture_cube_t* irradiance() const;
		texture_cube_t* prefiltered() const;
		math::vec3_t position() const;
		float radius() const;


		void set_irradiance(texture_cube_t* irradiance);
		void set_prefiltered(texture_cube_t* prefiltered);
	private:
		texture_cube_t* m_irradiance{};
		texture_cube_t* m_prefiltered{};
		math::vec3_t m_position{};
		float m_radius{};
	};
}
