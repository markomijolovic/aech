#pragma once
#include <any>

#include <unordered_map>


namespace aech::graphics
{
	class texture_t;
	class texture_cube_t;
	class shader_t;

	class material_t
	{
	public:
		enum class material_type
		{
			opaque,
			transparent
		};

		// make unordered_map happy
		material_t() = default;
		material_t(shader_t* m_shader, material_type m_type = material_type::opaque);
		shader_t*     shader() const;
		material_type type() const;

		// stores the uniform value in the material
		// note that this does not actually set it in the shader
		template <typename T>
		void set_uniform(const std::string& name, T&& value)
		{
			m_uniforms[name] = value;
		}

		template <typename T>
		T get_uniform(const std::string& name)
		{
			return m_uniforms[name];
		}

		// actually set the uniforms in the shader
		void set_uniforms() const;
		void set_texture(
			const std::string& name,
			const texture_t*   texture,
			uint32_t           unit);
		const texture_t* get_texture(
			const std::string& name);
		void set_texture_cube(
			const std::string&    name,
			const texture_cube_t* texture,
			uint32_t              unit);
		const texture_cube_t* get_texture_cube(
			const std::string& name);
		const std::unordered_map<std::string, std::pair<const texture_t*, uint32_t>>& get_textures() const;
	private:
		shader_t*                                                                   m_shader{};
		material_type                                                               m_type = material_type::opaque;
		std::unordered_map<std::string, std::any>                                   m_uniforms{};
		std::unordered_map<std::string, std::pair<const texture_t *, uint32_t>>     m_textures{};
		std::unordered_map<std::string, std::pair<const texture_cube_t*, uint32_t>> m_texture_cubes{};
	};
} // namespace aech::graphics
