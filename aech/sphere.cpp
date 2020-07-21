#include "sphere.hpp"
#include "aech_types.hpp"
#include "main.hpp"

// TODO(Marko): figure out how to calculate tangents and bitangents
aech::graphics::sphere_t::sphere_t(const uint32_t sectors, const uint32_t stacks)
{
    const auto sector_step = 2 * pi / sectors;
    const auto stack_step = pi / stacks;

    for (uint32_t i = 0; i <= stacks; i++) {
        // from pi/2 to - pi/2
        const auto stack_angle = pi / 2 - i * stack_step;
        const auto xy = cosf(stack_angle);
        const auto z = sinf(stack_angle);

        for (uint32_t j = 0; j <= sectors; j++) {
            // from 0 to 2pi
            const auto sector_angle = j * sector_step;
            auto x = xy * cosf(sector_angle);
            auto y = xy * sinf(sector_angle);

            auto s = static_cast<float>(j) / sectors;
            auto t = static_cast<float>(i) / stacks;
            m_uvs.emplace_back(s, t);

            auto nx = x;
            auto ny = y;
            auto nz = z;
            m_normals.emplace_back(nx, ny, nz);
            m_positions.emplace_back(x, y, z);
            //m_tangents.emplace_back(-sin(sector_angle), 0, cos(stack_angle));
            //m_bitangents.push_back(math::cross(m_normals[i], m_tangents[i]));
        }
    }

    //std::unordered_map<uint32_t, uint32_t> counts_per_vertex{};
    //m_tangents.resize(m_positions.size());
    //m_bitangents.resize(m_positions.size());
    for (uint32_t i = 0; i < stacks; i++) {
        auto k1 = i * (sectors + 1);
        auto k2 = k1 + sectors + 1;

        for (uint32_t j = 0; j < sectors; j++, k1++, k2++) {
            if (i != 0U) {
                // triangle (k1), (k2), (k1+1)
                m_indices.emplace_back(k1);
                m_indices.emplace_back(k2);
                m_indices.emplace_back(k1 + 1);

                /*auto e2 = m_positions[k1+1] - m_positions[k1];
				auto e1 = m_positions[k2] = m_positions[k1];

				auto delta2 = m_uvs[k1+1] - m_uvs[k2];
				auto delta1 = m_uvs[k2] - m_uvs[k1];
				
				math::vec3_t tangent
				{
					delta2.y * e1.x -delta1.y * e2.x,
					delta2.y * e1.y - delta1.y * e2.y,
					delta2.y * e1.z - delta1.y * e2.z
				};
				tangent /= (delta1.x * delta2.y - delta2.x * delta1.y);

				math::vec3_t bitangent
				{
					-delta2.x * e1.x +delta1.x * e2.x,
					-delta2.x * e1.y + delta1.x * e2.y,
					-delta2.x * e1.z + delta1.x * e2.z
				};
				bitangent /= (delta1.x * delta2.y - delta2.x * delta1.y);

				counts_per_vertex[k1]++;
				counts_per_vertex[k2]++;
				counts_per_vertex[k1+1]++;

				m_tangents[k1] += tangent;
				m_tangents[k2] += tangent;
				m_tangents[k1 + 1] += tangent;

				m_bitangents[k1] += bitangent;
				m_bitangents[k2] += bitangent;
				m_bitangents[k1 + 1] += bitangent;*/
            }
            if (i != stacks - 1) {
                // triangle (k1+1), (k2), (k2+1)
                m_indices.emplace_back(k1 + 1);
                m_indices.emplace_back(k2);
                m_indices.emplace_back(k2 + 1);

                /*auto e2 = m_positions[k2+1] - m_positions[k1+1];
				auto e1 = m_positions[k2] = m_positions[k1+1];

				auto delta2 = m_uvs[k2+1] - m_uvs[k2];
				auto delta1 = m_uvs[k2] - m_uvs[k1+1];
				
				math::vec3_t tangent
				{
					delta2.y * e1.x -delta1.y * e2.x,
					delta2.y * e1.y - delta1.y * e2.y,
					delta2.y * e1.z - delta1.y * e2.z
				};
				tangent /= (delta1.x * delta2.y - delta2.x * delta1.y);

				math::vec3_t bitangent
				{
					-delta2.x * e1.x +delta1.x * e2.x,
					-delta2.x * e1.y + delta1.x * e2.y,
					-delta2.x * e1.z + delta1.x * e2.z
				};
				bitangent /= (delta1.x * delta2.y - delta2.x * delta1.y);

				counts_per_vertex[k2+1]++;
				counts_per_vertex[k2]++;
				counts_per_vertex[k1+1]++;

				m_tangents[k2+1] += tangent;
				m_tangents[k2] += tangent;
				m_tangents[k1 + 1] += tangent;

				m_bitangents[k2+1] += bitangent;
				m_bitangents[k2] += bitangent;
				m_bitangents[k1 + 1] += bitangent;*/
            }
        }
    }

    //for (uint32_t i = 0; i < m_tangents.size(); i++)
    //{
    //	m_tangents[i] /= counts_per_vertex[i];
    //	m_tangents[i] =  math::normalize(m_tangents[i]);
    //	m_bitangents[i] /= counts_per_vertex[i];
    //	m_bitangents[i] = math::normalize(m_bitangents[i]);
    //}

    commit();
}
