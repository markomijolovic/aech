#pragma once
#include "opaque_shadow_renderer.hpp"
#include "directional_light_renderer.hpp"
#include "transparent_shadow_renderer.hpp"
#include "opaque_renderer.hpp"
#include "light_probe.hpp"
#include "point_light_renderer.hpp"
#include "light_probe_renderer.hpp"
#include "transparent_renderer.hpp"


namespace aech::graphics
{
	class renderer_t
	{
	public:
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
		framebuffer_cube_t*                            specular_prefilter_fbo{};
		framebuffer_t * post_process_fbo{};
		shader_t*                                      hdr_to_cubemap_shader{};
		shader_t*                                      irradiance_shader{};
		shader_t*                                      specular_prefilter_shader{};
		shader_t*                                      post_process_shader{};
		shader_t* tonemap_shader{};

		bool shadows{true};
		bool environment_mapping{true};
		bool fxaa{true};
		bool gui{};
		float poisson_sampling_distance {1.337F};

		void init();
		void render_gui();
		void update();
		void post_process();
		void bake_probes();
	};
} // namespace aech::graphics
