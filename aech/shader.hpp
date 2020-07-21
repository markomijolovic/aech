#pragma once

#include "mat.hpp"
#include <glad/glad.h>
#include <string>

namespace aech::graphics {
// a class that represents a shader program
class shader_t {
public:
    // make unordered_map happy
    shader_t() = default;

    // construct from the given vertex, fragment and geometry sources
    shader_t(
        const std::string& vertex_source,
        const std::string& fragment_source,
        const std::string& geometry_source);

    bool operator==(const shader_t& rhs) const;
    bool operator!=(const shader_t& rhs) const;

    void use() const;

    template <typename T>
    void set_uniform(const std::string& name, T&& value) const
    {
        using Type = std::remove_cvref_t<T>;
        if constexpr (std::is_same_v<Type, math::mat4_t>) {
            glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()),
                1,
                true,
                const_cast<GLfloat*>(reinterpret_cast<const GLfloat*>(&(value[0].data))));
        } else if constexpr (std::is_same_v<Type, math::vec2_t>) {
            glUniform2fv(glGetUniformLocation(m_id, name.c_str()),
                1,
                const_cast<GLfloat*>(reinterpret_cast<const GLfloat*>(&(value.data))));
        } else if constexpr (std::is_same_v<Type, math::vec3_t>) {
            glUniform3fv(glGetUniformLocation(m_id, name.c_str()),
                1,
                const_cast<GLfloat*>(reinterpret_cast<const GLfloat*>(&(value.data))));
        } else if constexpr (std::is_same_v<Type, math::vec4_t>) {
            glUniform4fv(glGetUniformLocation(m_id, name.c_str()),
                1,
                const_cast<GLfloat*>(reinterpret_cast<const GLfloat*>(&(value.data))));
        } else if constexpr (std::is_same_v<Type, bool>) {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), static_cast<GLint>(value));
        } else if constexpr (std::is_same_v<Type, int32_t>) {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
        } else if constexpr (std::is_same_v<Type, uint32_t>) {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), static_cast<int32_t>(value));
        } else if constexpr (std::is_same_v<Type, float>) {
            glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
        }
    }

private:
    uint32_t m_id;
    static void check_compile_errors(uint32_t id, const std::string& type);
};
} // namespace aech::graphics
