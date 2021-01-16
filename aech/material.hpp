#pragma once

#include <any>
#include <unordered_map>
#include <string>

namespace aech::graphics {
class texture_t;
class texture_cube_t;
class shader_t;

// a class that represents a material
class material_t {
public:
    enum class material_type {
        opaque,
        transparent
    };

    // make unordered_map happy
    material_t() = default;

    // construct from the given shader object
    material_t(shader_t *m_shader, material_type m_type = material_type::opaque);
    [[nodiscard]] auto shader() const -> shader_t *;
    [[nodiscard]] auto type() const -> material_type;

    // stores the uniform value in the material
    // note that this does not actually set it in the shader
    template <typename T>
    auto set_uniform(const std::string &name, T &&value) -> void
    {
        m_uniforms[name] = value;
    }

    template <typename T>
    auto get_uniform(const std::string &name) -> T
    {
        return m_uniforms[name];
    }

    // actually set the uniforms in the shader
    auto set_uniforms() const -> void;
    auto set_texture(
        const std::string &name,
        const texture_t *  texture,
        std::uint32_t           unit) -> void;
    auto get_texture(
        const std::string &name) -> const texture_t *;
    auto set_texture_cube(
        const std::string &   name,
        const texture_cube_t *texture,
        std::uint32_t              unit) -> void;
    auto get_texture_cube(
        const std::string &name) -> const texture_cube_t *;
    [[nodiscard]] auto get_textures() const -> const std::unordered_map<std::string, std::pair<const texture_t *, std::uint32_t>> &;

private:
    shader_t *                                                                   m_shader{};
    material_type                                                                m_type = material_type::opaque;
    std::unordered_map<std::string, std::any>                                    m_uniforms{};
    std::unordered_map<std::string, std::pair<const texture_t *, std::uint32_t>>      m_textures{};
    std::unordered_map<std::string, std::pair<const texture_cube_t *, std::uint32_t>> m_texture_cubes{};
};
} // namespace aech::graphics
