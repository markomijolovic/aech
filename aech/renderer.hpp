#pragma once
#include "gi_renderer.hpp"
#include "opaque_shadow_renderer.hpp"
#include "directional_light_renderer.hpp"
#include "transparent_shadow_renderer.hpp"
#include "opaque_renderer.hpp"
#include "light_probe.hpp"
#include "point_light_renderer.hpp"
#include "transparent_renderer.hpp"
#include "transforms.hpp"
#include "render_cache.hpp"


namespace aech::graphics
{
	class renderer_t
	{
	public:
		void init();
		void bake_probes();
		void update();

		[[nodiscard]] render_cache_t*  render_cache();
		[[nodiscard]] bool             ssao() const;
		[[nodiscard]] const texture_t* ssao_texture() const;
		[[nodiscard]] bool             shadows() const;
		[[nodiscard]] bool             options() const;
		[[nodiscard]] float            poisson_sampling_distance() const;
		void                           set_options(bool gui);
		
		[[nodiscard]] static bool sort_front_to_back(entity_t a, entity_t b);
		[[nodiscard]] static bool sort_back_to_front(entity_t a, entity_t b);		
		inline static auto light_projection{math::orthographic(-22.50, 22.50, -22.50, 20.00, 0, 22.50)};

	private:
		inline static constexpr uint32_t ssao_kernel_size{32};

		camera_t* m_camera{};
		std::shared_ptr<point_light_renderer_t>        point_light_renderer{};
		std::shared_ptr<opaque_shadow_renderer_t>      opaque_shadow_renderer{};
		std::shared_ptr<transparent_shadow_renderer_t> transparent_shadow_renderer{};
		std::shared_ptr<transparent_renderer_t>        transparent_renderer{};
		std::shared_ptr<gi_renderer_t>				   gi_renderer{};
		std::shared_ptr<opaque_renderer_t>             opaque_renderer{};
		std::unique_ptr<directional_light_renderer_t>  m_directional_light_renderer{};
		std::vector<reflection_probe_t>                     probes{};
		render_cache_t                                 m_render_cache{};
		mesh_t*                                        ndc_cube{};
		mesh_t*                                        screen_quad{};
		framebuffer_cube_t*                            hdr_capture_fbo{};
		framebuffer_cube_t*                            irradiance_fbo{};
		framebuffer_t*                                 post_process_fbo{};
		framebuffer_t*                                 m_ssao_fbo{};
		framebuffer_t*                                 m_ssao_blurred_fbo{};
		framebuffer_t*                                 m_gbuffer{};
		shader_t*                                      hdr_to_cubemap_shader{};
		shader_t*                                      irradiance_shader{};
		shader_t*                                      specular_prefilter_shader{};
		shader_t*                                      post_process_shader{};
		shader_t*                                      tonemap_shader{};

		shader_t* m_ssao_shader{};
		shader_t* m_ssao_blur_shader{};

		std::unique_ptr<texture_t> ssao_noise_texture{};
		std::vector<math::vec3_t>  ssao_kernel{};

		bool  m_shadows{true};
		float m_poisson_sampling_distance{1.337F};
		float m_ssao_hemisphere_sampling_radius{5.0F};
		bool  fxaa{true};
		bool  m_ssao{true};
		bool  m_options{};

		void render_ssao();
		void render_gui();
		void post_process();
	};
} // namespace aech::graphics
