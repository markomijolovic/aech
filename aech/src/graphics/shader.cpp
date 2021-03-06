#include "shader.hpp"

#include <iostream>

namespace aech::graphics {
shader_t::shader_t(
    const std::string &vertex_source,
    const std::string &fragment_source,
    const std::string &geometry_source) noexcept
{
    const auto s_vertex    = glCreateShader(GL_VERTEX_SHADER);
    const auto vertex_data = vertex_source.c_str();
    glShaderSource(s_vertex, 1, &vertex_data, nullptr);
    glCompileShader(s_vertex);
    check_compile_errors(s_vertex, "vertex");

    const auto s_fragment    = glCreateShader(GL_FRAGMENT_SHADER);
    const auto fragment_data = fragment_source.c_str();
    glShaderSource(s_fragment, 1, &fragment_data, nullptr);
    glCompileShader(s_fragment);
    assert(check_compile_errors(s_fragment, "fragment"));

    std::uint32_t s_geometry{};
    if (!geometry_source.empty()) {
        s_geometry               = glCreateShader(GL_GEOMETRY_SHADER);
        const auto geometry_data = geometry_source.c_str();
        glShaderSource(s_geometry, 1, &geometry_data, nullptr);
        glCompileShader(s_geometry);
        assert(check_compile_errors(s_geometry, "geometry"));
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, s_vertex);
    glAttachShader(m_id, s_fragment);

    if (!geometry_source.empty()) {
        glAttachShader(m_id, s_vertex);
    }

    glLinkProgram(m_id);
    assert(check_compile_errors(m_id, "program"));

    glDeleteShader(s_vertex);
    glDeleteShader(s_fragment);
    if (!geometry_source.empty()) {
        glDeleteShader(s_geometry);
    }
}

auto shader_t::use() const noexcept -> void
{
    assert(m_id);

    glUseProgram(m_id);
}

auto shader_t::check_compile_errors(std::uint32_t id, const std::string &type) noexcept -> bool
{
    GLint success{};
    if (type == "program") {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (success == 0) {
            GLchar info_log[1 << 10];
            glGetProgramInfoLog(id, 1 << 10, nullptr, info_log);
            std::clog << "ERROR::PROGRAM_LINKING_ERROR: " << info_log << std::endl;
        }
    } else {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (success == 0) {
            GLchar info_log[1 << 10];
            glGetShaderInfoLog(id, 1 << 10, nullptr, info_log);
            std::clog << "ERROR::SHADER_COMPILATION_ERROR(" << type << ")\n"
                      << info_log << std::endl;
        }
    }

    return success != 0;
}
} // namespace aech::graphics
