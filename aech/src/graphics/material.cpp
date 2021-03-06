#include "material.hpp"

#include "mat.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "texture_cube.hpp"

// TODO(Marko): Look into using fewer glUseProgram calls
namespace aech::graphics {
auto material_t::get_textures() noexcept -> std::unordered_map<std::string, std::pair<const texture_t *, std::uint32_t>> &
{
    return m_textures;
}

auto material_t::get_texture(const std::string &name) noexcept -> const texture_t *
{
    if (m_textures.find(name) != std::end(m_textures)) {
        return m_textures[name].first;
    }
    return nullptr;
}

auto material_t::get_texture_cube(const std::string &name) noexcept -> const texture_cube_t *
{
    if (m_texture_cubes.find(name) != std::end(m_texture_cubes)) {
        return m_texture_cubes[name].first;
    }
    return nullptr;
}

material_t::material_t(shader_t *m_shader, material_type m_type) noexcept
    : m_shader{m_shader}
    , m_type{m_type}
{
}

auto material_t::shader() const noexcept -> shader_t &
{
    assert(m_shader);

    return *m_shader;
}

auto material_t::type() const noexcept -> material_t::material_type
{
    return m_type;
}

auto material_t::set_texture(const std::string &name, const texture_t *texture, std::uint32_t unit) noexcept -> void
{
    m_textures[name] = {texture, unit};
}

auto material_t::set_texture_cube(const std::string &name, const texture_cube_t *texture, std::uint32_t unit) noexcept -> void
{
    m_texture_cubes[name] = {texture, unit};
}

auto material_t::set_uniforms() const noexcept -> void
{
    m_shader->use();

    for (const auto &el: m_textures) {
        el.second.first->bind(el.second.second);
        m_shader->set_uniform(el.first, el.second.second);
    }

    for (const auto &el: m_texture_cubes) {
        el.second.first->bind(el.second.second);
        m_shader->set_uniform(el.first, el.second.second);
    }

    for (const auto &el: m_uniforms) {
        if (el.second.type() == typeid(math::mat4_t)) {
            m_shader->set_uniform(el.first, std::any_cast<math::mat4_t>(el.second));
        } else if (el.second.type() == typeid(math::vec3_t)) {
            m_shader->set_uniform(el.first, std::any_cast<math::vec3_t>(el.second));
        } else if (el.second.type() == typeid(math::vec4_t)) {
            m_shader->set_uniform(el.first, std::any_cast<math::vec4_t>(el.second));
        } else if (el.second.type() == typeid(bool)) {
            m_shader->set_uniform(el.first, std::any_cast<bool>(el.second));
        } else if (el.second.type() == typeid(std::int32_t)) {
            m_shader->set_uniform(el.first, std::any_cast<std::int32_t>(el.second));
        } else if (el.second.type() == typeid(std::uint32_t)) {
            m_shader->set_uniform(el.first, std::any_cast<std::uint32_t>(el.second));
        } else if (el.second.type() == typeid(float)) {
            m_shader->set_uniform(el.first, std::any_cast<float>(el.second));
        } // namespace aech::graphics
    }
}
} // namespace aech::graphics
