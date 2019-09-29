#include "resource_manager.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "main.hpp"
#include "material_library.hpp"
#include "mesh.hpp"
#include "mesh_filter.hpp"
#include "scene_node.hpp"
#include "shading_tags.hpp"

#include "shadow_caster.hpp"

#include "stb_image.h"

#include "texture_cube.hpp"

#include "transform.hpp"

#include <assimp/postprocess.h>

#include <fstream>

#include <iostream>

#include <sstream>


namespace aech::resource_manager
{
	using namespace graphics;

	shader_t& load_shader(const std::string& name,
	                      const std::string& vertex,
	                      const std::string& fragment,
	                      const std::string& geometry)
	{
		if (shaders.find(name) != std::end(shaders))
		{
			return shaders[name];
		}

		std::clog << "Loading shader " << name << std::endl;

		std::string vertex_code{};
		std::string fragment_code{};
		std::string geometry_code{};

		try
		{
			std::ifstream vertex_file{vertex};
			std::ifstream fragment_file{fragment};

			std::stringstream vertex_stream{};
			std::stringstream fragment_stream{};

			vertex_stream << vertex_file.rdbuf();
			fragment_stream << fragment_file.rdbuf();

			vertex_code   = vertex_stream.str();
			fragment_code = fragment_stream.str();

			if (!geometry.empty())
			{
				std::ifstream     geometry_file{geometry};
				std::stringstream geometry_stream{};

				geometry_stream << geometry_file.rdbuf();
				geometry_code = geometry_stream.str();
			}
		}
		catch (const std::exception& exc)
		{
			std::clog << exc.what() << std::endl;
		}

		return shaders[name] = shader_t{vertex_code, fragment_code, geometry_code};
	}

	texture_cube_t* load_texture_cube(const std::string& name,
	                                  const std::string& top,
	                                  const std::string& bottom,
	                                  const std::string& left,
	                                  const std::string& right,
	                                  const std::string& front,
	                                  const std::string& back,
	                                  bool               srgb)
	{
		if (texture_cubes.find(name) != std::end(texture_cubes))
		{
			return &texture_cubes[name];
		}

		std::clog << "Loading cubemap texture " << name << " from " << top << ", " << bottom << ", " << left << ", " <<
			right << ", " << front << ", " << back << std::endl;

		auto& texture = texture_cubes[name];

		stbi_set_flip_vertically_on_load(0);

		std::vector<std::string> faces{top, bottom, left, right, front, back};


		for (uint32_t i = 0; i < faces.size(); i++)
		{
			int32_t    width;
			int32_t    height;
			int32_t    number_of_components;
			const auto data = stbi_load(faces[i].c_str(), &width, &height, &number_of_components, 0);
			if (data != nullptr)
			{
				texture_types::sized_internal_format sized_internal_format{};
				texture_types::format                format{};

				switch (number_of_components)
				{
					case 1:
						sized_internal_format = texture_types::sized_internal_format::r8;
						format = texture_types::format::r;
						break;
					case 2:
						sized_internal_format = texture_types::sized_internal_format::rg8;
						format = texture_types::format::rg;
						break;
					case 3:
						sized_internal_format = srgb
							                        ? texture_types::sized_internal_format::srgb8
							                        : texture_types::sized_internal_format::rgb8;
						format = texture_types::format::rgb;
						break;
					case 4:
						sized_internal_format = srgb
							                        ? texture_types::sized_internal_format::srgb8a8
							                        : texture_types::sized_internal_format::rgba8;
						format = texture_types::format::rgba;
						break;
					default:
						break;
				}

				if (i == 0)
				{
					texture = texture_cube_t{
						static_cast<uint32_t>(width),
						static_cast<uint32_t>(height),
						sized_internal_format,
						format
					};
				}

				texture.generate_face(i,
				                      data);
				stbi_image_free(data);
			}
			else
			{
				std::clog << "Cubemap texture " << name << " failed to load" << std::endl;
				stbi_image_free(data);
			}
		}

		texture.generate_mips();

		return &texture_cubes[name];
	}

	texture_t* load_texture(const std::string& name,
	                        const std::string& path,
	                        bool               srgb)
	{
		if (textures.find(name) != std::end(textures))
		{
			return &textures[name];
		}

		std::clog << "Loading texture " << name << " from " << path << std::endl;

		stbi_set_flip_vertically_on_load(1);

		int32_t    width;
		int32_t    height;
		int32_t    number_of_components;
		const auto data = stbi_load(path.c_str(), &width, &height, &number_of_components, 0);

		if (data != nullptr)
		{
			texture_types::format                format{};
			texture_types::sized_internal_format sized_internal_format{};

			switch (number_of_components)
			{
				case 1:
					sized_internal_format = texture_types::sized_internal_format::r8;
					format = texture_types::format::r;
					break;
				case 2:
					sized_internal_format = texture_types::sized_internal_format::rg8;
					format = texture_types::format::rg;
					break;
				case 3:
					sized_internal_format = srgb
						                        ? texture_types::sized_internal_format::srgb8
						                        : texture_types::sized_internal_format::rgb8;
					format = texture_types::format::rgb;
					break;
				case 4:
					sized_internal_format = srgb
						                        ? texture_types::sized_internal_format::srgb8a8
						                        : texture_types::sized_internal_format::rgba8;
					format = texture_types::format::rgba;
					break;
				default:
					break;
			}

			auto& texture = textures[name] = texture_t{
				                static_cast<uint32_t>(width),
				                static_cast<uint32_t>(height),
				                sized_internal_format,
				                format,
				                texture_types::type::ubyte,
				                true,
				                texture_types::filtering::linear_mipmap_linear,
				                texture_types::filtering::linear,
				                data,
			                };
		}
		else
		{
			std::clog << "Failed to load texture " << name << " from " << path << std::endl;
		}
		stbi_image_free(data);

		return &textures[name];
	}

	texture_t* load_hdr_texture(const std::string& name, const std::string& path)
	{
		if (textures.find(name) != std::end(textures))
		{
			return &textures[name];
		}

		std::clog << "Loading hdr texture " << name << " from " << path << std::endl;

		stbi_set_flip_vertically_on_load(1);

		int32_t    width;
		int32_t    height;
		int32_t    number_of_components;
		const auto data = stbi_loadf(path.c_str(), &width, &height, &number_of_components, 0);

		if (data != nullptr)
		{
			texture_types::format                format{};
			texture_types::sized_internal_format sized_internal_format{};

			switch (number_of_components)
			{
				case 1:
					sized_internal_format = texture_types::sized_internal_format::r32f;
					format = texture_types::format::r;
					break;
				case 2:
					sized_internal_format = texture_types::sized_internal_format::rg32f;
					format = texture_types::format::rg;
					break;
				case 3:
					sized_internal_format = texture_types::sized_internal_format::rgb32f;
					format = texture_types::format::rgb;
					break;
				case 4:
					sized_internal_format = texture_types::sized_internal_format::rgba32f;
					format = texture_types::format::rgba;
					break;
				default:
					break;
			}

			auto& texture = textures[name] = texture_t{
				                static_cast<uint32_t>(width),
				                static_cast<uint32_t>(height),
				                sized_internal_format,
				                format,
				                texture_types::type::floating_point,
				                true,
				                texture_types::filtering::linear_mipmap_linear,
				                texture_types::filtering::linear,
				                data,
			                };
		}
		else
		{
			std::clog << "Failed to load hdr texture " << name << " from " << path << std::endl;
		}
		stbi_image_free(data);

		return &textures[name];
	}

	entity_t process_node(const aiNode* node, const aiScene* scene)
	{
		const auto entity = engine.create_entity();
		engine.add_component(entity,
		                     transform_t{}
		                    );
		const auto transform = &engine.get_component<transform_t>(entity);
		engine.add_component(entity,
		                     scene_node_t{transform}
		                    );
		if (node->mNumMeshes == 1)
		{
			engine.add_component(entity,
			                     potential_occluder_t{}
			                    );
		}

		auto scene_node = &engine.get_component<scene_node_t>(entity);

		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			const auto a_mesh      = scene->mMeshes[node->mMeshes[i]];
			const auto mesh        = parse_mesh(a_mesh, scene);
			const auto a_material  = scene->mMaterials[a_mesh->mMaterialIndex];
			const auto material    = parse_material(scene, a_material);
			auto&      mesh_filter = engine.get_component<mesh_filter_t>(entity);
			if (node->mNumMeshes == 1)
			{
				mesh_filter_t mesh_filter{mesh, material};
				engine.add_component(entity,
				                     mesh_filter
				                    );
				if (mesh_filter.material()->type() == material_t::material_type::opaque)
				{
					engine.add_component(entity, opaque_t{});
				}
				else
				{
					engine.add_component(entity, transparent_t{});
				}
			}
			else
			{
				const auto child_entity = engine.create_entity();
				engine.add_component(child_entity,
				                     transform_t{}
				                    );
				const auto child_transform = &engine.get_component<transform_t>(child_entity);
				engine.add_component(
				                     child_entity,
				                     scene_node_t{child_transform}
				                    );


				engine.add_component(child_entity,
				                     potential_occluder_t{}
				                    );
				mesh_filter_t mesh_filter{mesh, material};
				engine.add_component(child_entity,
				                     mesh_filter
				                    );
				if (mesh_filter.material()->type() == material_t::material_type::opaque)
				{
					engine.add_component(child_entity, opaque_t{});
				}
				else
				{
					engine.add_component(child_entity, transparent_t{});
				}

				const auto child_scene_node  = &engine.get_component<scene_node_t>(child_entity);
				auto&      child_mesh_filter = engine.get_component<mesh_filter_t>(child_entity);
				scene_node->add_child(child_scene_node);
			}
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			const auto child_id         = process_node(node->mChildren[i], scene);
			const auto child_scene_node = &engine.get_component<scene_node_t>(child_id);
			scene_node->add_child(child_scene_node);
		}

		return entity;
	}

	entity_t load_mesh(const std::string& path)
	{
		std::clog << "Loading mesh " << path << std::endl;
		Assimp::Importer importer{};
		const auto*      scene = importer.ReadFile(path,
		                                           aiProcess_Triangulate |
		                                           aiProcess_CalcTangentSpace
		                                          );

		if (scene == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
		{
			std::clog << "Failed to load mesh from " << path << std::endl;
			return invalid_entity_id;
		}

		return process_node(scene->mRootNode, scene);
	}

	mesh_t* parse_mesh(aiMesh* mesh, const aiScene* /*scene*/)
	{
		const auto it = meshes.find(mesh);
		if (it != std::end(meshes))
		{
			return &meshes[mesh];
		}

		auto ret_val = &meshes[mesh];

		std::vector<math::vec3_t> positions(mesh->mNumVertices);
		std::vector<math::vec3_t> normals(mesh->mNumVertices);
		std::vector<math::vec3_t> tangents{};
		std::vector<math::vec3_t> bitangents{};
		std::vector<math::vec2_t> uvs{};
		std::vector<uint32_t>     indices(3 * mesh->mNumFaces);
		bounding_box_t            aabb{};

		if (mesh->HasTextureCoords(0))
		{
			uvs.resize(mesh->mNumVertices);
			tangents.resize(mesh->mNumVertices);
			bitangents.resize(mesh->mNumVertices);
		}

		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			positions[i] = math::vec3_t{
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};

			aabb.min_coords.x = std::min(aabb.min_coords.x, positions[i].x);
			aabb.min_coords.y = std::min(aabb.min_coords.y, positions[i].y);
			aabb.min_coords.z = std::min(aabb.min_coords.z, positions[i].z);

			aabb.max_coords.x = std::max(aabb.max_coords.x, positions[i].x);
			aabb.max_coords.y = std::max(aabb.max_coords.y, positions[i].y);
			aabb.max_coords.z = std::max(aabb.max_coords.z, positions[i].z);

			normals[i] = math::vec3_t{
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};

			if (mesh->HasTextureCoords(0))
			{
				uvs[i] = math::vec2_t{
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			}

			if (mesh->mTangents != nullptr)
			{
				tangents[i] = math::vec3_t{
					mesh->mTangents[i].x,
					mesh->mTangents[i].y,
					mesh->mTangents[i].z
				};
				bitangents[i] = math::vec3_t{
					mesh->mBitangents[i].x,
					mesh->mBitangents[i].y,
					mesh->mBitangents[i].z
				};
			}
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				indices[i * 3 + j] = mesh->mFaces[i].mIndices[j];
			}
		}

		*ret_val = mesh_t
		{
			positions,
			normals,
			aabb,
			uvs,
			mesh_t::topology::triangles,
			indices,
			tangents,
			bitangents
		};

		return ret_val;
	}


	texture_cube_t* load_texture_cube(const std::string& name, const std::string& folder)
	{
		return load_texture_cube(
		                         name,
		                         folder + "right.jpg",
		                         folder + "left.jpg",
		                         folder + "top.jpg",
		                         folder + "bottom.jpg",
		                         folder + "front.jpg",
		                         folder + "back.jpg"
		                        );
	}


	material_t* parse_material(const aiScene* /*scene*/, aiMaterial* a_material)
	{
		const auto it = materials.find(a_material);
		if (it != std::end(materials))
		{
			return &materials[a_material];
		}

		auto ret_val = &materials[a_material];

		aiString file{};
		a_material->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		auto path = std::string{file.C_Str()};
		if (path.find("_alpha") != std::string::npos)
		{
			*ret_val = material_library::create_material("transparent");
		}
		else
		{
			*ret_val = material_library::create_material("default");
		}

		if (a_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString file{};
			a_material->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			auto file_name = std::string{file.C_Str()};

			auto texture = load_texture(file_name, file_name);
			ret_val->set_texture("texture_albedo", texture, 0);
		}


		if (a_material->GetTextureCount(aiTextureType_HEIGHT) > 0)
		{
			aiString file{};
			a_material->GetTexture(aiTextureType_HEIGHT, 0, &file);
			auto file_name = std::string{file.C_Str()};

			auto texture = load_texture(file_name, file_name, false);
			ret_val->set_texture("texture_normal", texture, 1);
		}

		if (a_material->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			aiString file{};
			a_material->GetTexture(aiTextureType_SPECULAR, 0, &file);
			auto file_name = std::string{file.C_Str()};

			auto texture = load_texture(file_name, file_name, false);
			ret_val->set_texture("texture_metallic", texture, 2);
		}

		if (a_material->GetTextureCount(aiTextureType_SHININESS) > 0)
		{
			aiString file{};
			a_material->GetTexture(aiTextureType_SHININESS, 0, &file);
			auto file_name = std::string{file.C_Str()};

			auto texture = load_texture(file_name, file_name, false);
			ret_val->set_texture("texture_roughness", texture, 3);
		}

		if (a_material->GetTextureCount(aiTextureType_AMBIENT) > 0)
		{
			aiString file{};
			a_material->GetTexture(aiTextureType_AMBIENT, 0, &file);
			auto file_name = std::string{file.C_Str()};

			auto texture = load_texture(file_name, file_name, false);
			ret_val->set_texture("texture_ao", texture, 4);
		}

		return ret_val;
	}
} // namespace aech::resource_manager
