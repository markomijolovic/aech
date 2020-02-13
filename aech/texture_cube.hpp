#pragma once

#include "aech_types.hpp"
#include "texture_types.hpp"

namespace aech::graphics
{
	class texture_cube_t
	{
	public:
		// make unordered_map happy
		texture_cube_t() = default;

		texture_cube_t(uint32_t                             width,
		               uint32_t                             height,
		               texture_types::sized_internal_format sif    = texture_types::sized_internal_format::rgba8,
		               texture_types::format                f      = texture_types::format::rgba,
		               texture_types::type                  t      = texture_types::type::ubyte,
		               bool                                 mipmap = true,
		               texture_types::filtering             min    = texture_types::filtering::linear,
		               texture_types::filtering             mag    = texture_types::filtering::linear);
		texture_cube_t& operator=(texture_cube_t&& rhs) noexcept;
		texture_cube_t(texture_cube_t&& rhs) noexcept;
		~texture_cube_t();
		
		[[nodiscard]] uint32_t id() const;
		void                   generate_face(uint32_t index, void* data) const;
		void                   generate_mips() const;

		void        bind(int32_t unit = -1) const;
		static void unbind();

	private:

		uint32_t                             m_id{};
		uint32_t                             m_width{};
		uint32_t                             m_height{};
		texture_types::sized_internal_format m_sized_internal_format = texture_types::sized_internal_format::rgba8;
		texture_types::format                m_format                = texture_types::format::rgba;
		texture_types::type                  m_type                  = texture_types::type::ubyte;
		texture_types::filtering             m_filtering_min         = texture_types::filtering::linear;
		texture_types::filtering             m_filtering_mag         = texture_types::filtering::linear;
		texture_types::sampling              m_wrap_s                = texture_types::sampling::clamp_to_edge;
		texture_types::sampling              m_wrap_t                = texture_types::sampling::clamp_to_edge;
		texture_types::sampling              m_wrap_r                = texture_types::sampling::clamp_to_edge;
		bool                                 m_mipmap                = true;
	};
} // namespace aech::graphics
