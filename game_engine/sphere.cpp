#include "sphere.hpp"

aech::sphere_t::sphere_t(const uint32_t sectors, const uint32_t stacks, const float radius)
{
	const auto sector_step = 2 * pi / sectors;
	const auto stack_step  = pi / stacks;
	const auto length_inv = 1 / radius;

	for (uint32_t i = 0; i <= stacks; i++)
	{
		// from pi/2 to - pi/2
		const auto stack_angle = pi / 2 - i * stack_step;
		const auto xy          = radius * cosf(stack_angle);
		const auto z           = radius * sinf(stack_angle);

		for (uint32_t j = 0; j <= sectors; j++)
		{
			// from 0 to 2pi
			const auto sector_angle = j * sector_step;

			auto x = xy * cosf(sector_angle);
			auto y = xy * sinf(sector_angle);

			m_positions.emplace_back(x, y, z);

			auto nx = x * length_inv;
			auto ny = y * length_inv;
			auto nz = z * length_inv;

			m_normals.emplace_back(nx, ny, nz);

			auto s = static_cast<float>(j) / sectors;
			auto t = static_cast<float>(i) / stacks;

			m_uvs.emplace_back(s, t);
		}
	}

	/*for (uint32_t i = 0; i < stacks; i++)
	{
		auto k1 = i * (sectors + 1);
		auto k2 = k1 + sectors + 1;

		for (uint32_t j = 0; j < sectors; j++, k1++, k2++)
		{
			if (i != 0U)
			{
				m_indices.emplace_back(k1);
				m_indices.emplace_back(k2);
				m_indices.emplace_back(k1 + 1);
			}

			if (i != stacks - 1)
			{
				m_indices.emplace_back(k1 + 1);
				m_indices.emplace_back(k2);
				m_indices.emplace_back(k2 + 1);
			}
		}

	}
*/
	commit(true);

}