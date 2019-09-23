#pragma once
#include "opaque_shadow_renderer.hpp"
#include "directional_light_renderer.hpp"
#include "transparent_shadow_renderer.hpp"
#include "opaque_renderer.hpp"
#include "light_probe.hpp"
#include "point_light_renderer.hpp"
#include "light_probe_renderer.hpp"
#include "transparent_renderer.hpp"
#include "transforms.hpp"


namespace aech::graphics
{
	class renderer_t
	{
	public:
		void init();
		void bake_probes();
		void update();

		[[nodiscard]] bool  shadows() const;
		[[nodiscard]] bool  options() const;
		[[nodiscard]] float poisson_sampling_distance() const;
		void                set_options(bool gui);

		inline static auto light_projection{math::orthographic(-2250, 2250, -2250, 2000, 0, 2250)};

	private:
		entity_t                                       m_camera{};
		std::shared_ptr<directional_light_renderer_t>  directional_light_renderer{};
		std::shared_ptr<point_light_renderer_t>        point_light_renderer{};
		std::shared_ptr<opaque_shadow_renderer_t>      opaque_shadow_renderer{};
		std::shared_ptr<transparent_shadow_renderer_t> transparent_shadow_renderer{};
		std::shared_ptr<transparent_renderer_t>        transparent_renderer{};
		std::shared_ptr<light_probe_renderer_t>        light_probe_renderer{};
		std::shared_ptr<opaque_renderer_t>             opaque_renderer{};
		std::vector<light_probe_t>                     probes{};
		mesh_t*                                        ndc_cube{};
		mesh_t*                                        screen_quad{};
		framebuffer_cube_t*                            hdr_capture_fbo{};
		framebuffer_cube_t*                            irradiance_fbo{};
		framebuffer_t*                                 post_process_fbo{};
		shader_t*                                      hdr_to_cubemap_shader{};
		shader_t*                                      irradiance_shader{};
		shader_t*                                      specular_prefilter_shader{};
		shader_t*                                      post_process_shader{};
		shader_t*                                      tonemap_shader{};

		bool  m_shadows{true};
		bool  environment_mapping{false};
		bool  fxaa{true};
		bool  m_options{};
		float m_poisson_sampling_distance{1.337F};

		void render_gui();
		void post_process() const;
	};
} // namespace aech::graphics
