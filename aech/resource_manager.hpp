#pragma once

#include "aech_types.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "texture_cube.hpp"

#include <assimp/material.h>
#include <string>
#include <unordered_map>

struct aiMesh;
struct aiNode;
struct aiScene;

namespace aech::resource_manager {
// shader storage, identified by string name
inline std::unordered_map<std::string, graphics::shader_t> shaders{};

// texture storage, identified by string name
inline std::unordered_map<std::string, graphics::texture_t> textures{};

// cubemap texture storage, identified by string name
inline std::unordered_map<std::string, graphics::texture_cube_t> texture_cubes{};

// these 2 are only used during loading process
// to avoid duplicating meshes and materials.
// after the loading process the pointers are actually destroyed
// but the mesh filter components maintans the pointers to the
// referenced meshes and materials
inline std::unordered_map<aiMesh *, graphics::mesh_t>         meshes{};
inline std::unordered_map<aiMaterial *, graphics::material_t> materials{};

auto load_mesh(const std::string &path) -> entity_t;
auto process_node(const aiNode *node, const aiScene *scene) -> entity_t;
auto parse_mesh(aiMesh *mesh, const aiScene *scene) -> graphics::mesh_t *;

// load the given vertex, fragment and geometry shaders
// and create a shader object identified by the given string name
auto load_shader(
    const std::string &name,
    const std::string &vertex,
    const std::string &fragment,
    const std::string &geometry = {}) -> graphics::shader_t &;

// create a texture object identified by the given string name
// path contains an LDR image
auto load_texture(const std::string &name, const std::string &path, bool srgb = true) -> graphics::texture_t *;

// create a texture object identified by the given string name
// path contains an HDR image
auto load_hdr_texture(const std::string &name, const std::string &path) -> graphics::texture_t *;

// create a cubemap texture identified by the given stirng name
auto load_texture_cube(const std::string &name,
                       const std::string &top,
                       const std::string &bottom,
                       const std::string &left,
                       const std::string &right,
                       const std::string &front,
                       const std::string &back,
                       bool               srgb = true) -> graphics::texture_cube_t *;

// create a cubemap texture identified by the given string name
auto load_texture_cube(const std::string &name, const std::string &folder) -> graphics::texture_cube_t *;

// create a material object from the given aiMaterial (Assimp class)
auto parse_material(const aiScene *scene, aiMaterial *material) -> graphics::material_t *;
} // namespace aech::resource_manager
