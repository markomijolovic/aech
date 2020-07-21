#pragma once

#include <glad/glad.h>

namespace aech::graphics::texture_types {
enum class sized_internal_format {
    r8 = GL_R8,
    rg8 = GL_RG8,
    rgb8 = GL_RGB8,
    rgba8 = GL_RGBA8,
    r16f = GL_R16F,
    rg16f = GL_RG16F,
    rgb16f = GL_RGB16F,
    rgba16f = GL_RGBA16F,
    r32f = GL_R32F,
    rg32f = GL_RG32F,
    rgb32f = GL_RGB32F,
    srgb8 = GL_SRGB8,
    rgba32f = GL_RGBA32F,
    srgb8a8 = GL_SRGB8_ALPHA8,
    depth24 = GL_DEPTH_COMPONENT24,
    depth32 = GL_DEPTH_COMPONENT32,
    depth32f = GL_DEPTH_COMPONENT32F
};

enum class format {
    r = GL_RED,
    rg = GL_RG,
    rgb = GL_RGB,
    rgba = GL_RGBA,
    depth = GL_DEPTH_COMPONENT
};

enum class type {
    ubyte = GL_UNSIGNED_BYTE,
    floating_point = GL_FLOAT,
    uint8888 = GL_UNSIGNED_INT_8_8_8_8
};

enum class filtering {
    linear = GL_LINEAR,
    linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR,
    nearest = GL_NEAREST
};

enum class sampling {
    repeat = GL_REPEAT,
    clamp_to_edge = GL_CLAMP_TO_EDGE
};

enum class target {
    oned = GL_TEXTURE_1D,
    twod = GL_TEXTURE_2D,
    threed = GL_TEXTURE_3D,
    cube_map = GL_TEXTURE_CUBE_MAP
};
} // namespace aech::graphics::texture_types
