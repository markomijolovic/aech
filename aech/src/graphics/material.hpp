#pragma once

#include <any>
#include <string>
#include <unordered_map>

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
    explicit material_t(shader_t *m_shader, material_type m_type = material_type::opaque) noexcept;
    [[nodiscard]] auto shader() const noexcept -> shader_t &;
    [[nodiscard]] auto type() const noexcept -> material_type;

    // stores the uniform value in the material
    // note that this does not actually set it in the shader
    template <typename T>
    auto set_uniform(const std::string &name, T &&value) noexcept -> void
    {
        m_uniforms[name] = value;
    }

    template <typename T>
    [[nodiscard]] auto get_uniform(const std::string &name) noexcept -> T
    {
        return m_uniforms[name];
    }

    // actually set the uniforms in the shader
    auto set_uniforms() const noexcept -> void;
    auto set_texture(
        const std::string &name,
        const texture_t *  texture,
        std::uint32_t      unit) noexcept -> void;
    [[nodiscard]] auto get_texture(
        const std::string &name) noexcept -> const texture_t *;
    auto set_texture_cube(
        const std::string &   name,
        const texture_cube_t *texture,
        std::uint32_t         unit) noexcept -> void;
    [[nodiscard]] auto get_texture_cube(
        const std::string &name) noexcept -> const texture_cube_t *;
    [[nodiscard]] auto get_textures() noexcept -> std::unordered_map<std::string, std::pair<const texture_t *, std::uint32_t>> &;

private:
    // NOTE: this is a NON-OWNING pointer!!!
    shader_t *                                                                        m_shader{};
    material_type                                                                     m_type = material_type::opaque;
    std::unordered_map<std::string, std::any>                                         m_uniforms{};
    std::unordered_map<std::string, std::pair<const texture_t *, std::uint32_t>>      m_textures{};
    std::unordered_map<std::string, std::pair<const texture_cube_t *, std::uint32_t>> m_texture_cubes{};
};
} // namespace aech::graphics
