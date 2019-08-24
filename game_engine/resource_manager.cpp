#include "resource_manager.hpp"
#include <iostream>
#include "mesh.hpp"
#include <fstream>
#include <sstream>
#include "stb_image.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include <assimp/postprocess.h>
#include "texture_cube.hpp"
#include "scene_node.hpp"
#include "transform.hpp"
#include "mesh_filter.hpp"

aech::shader_t& aech::resource_manager::load_shader(const std::string& vertex,
	const std::string& fragment,
	const std::string& geometry,
	const std::string& name)
{
	if (shaders.find(name) != std::end(shaders))
	{
		return shaders[name];
	}

	std::string vertex_code{};
	std::string fragment_code{};
	std::string geometry_code{};

	try
	{
		std::ifstream vertex_file{ vertex };
		std::ifstream fragment_file{ fragment };

		std::stringstream vertex_stream{};
		std::stringstream fragment_stream{};

		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();

		if (!geometry.empty())
		{
			std::ifstream     geometry_file{ geometry };
			std::stringstream geometry_stream{};

			geometry_stream << geometry_file.rdbuf();
			geometry_code = geometry_stream.str();
		}
	}
	catch (const std::exception& exc)
	{
		std::cerr << exc.what() << '\n';
	}

	shaders[name] = shader_t{};
	shaders[name].compile(vertex_code, fragment_code, geometry_code);
	return shaders[name];
}

aech::shader_t& aech::resource_manager::get_shader(const std::string& name)
{
	return shaders[name];
}


aech::texture_t& aech::resource_manager::load_texture(const std::string& name,
	const std::string& path,
	const GLenum target,
	const GLenum format,
	const bool srgb)
{
	if (textures.find(name) != std::end(textures))
	{
		return textures[name];
	}

	texture_t texture{};
	texture.m_target = target;
	texture.m_internal_format = format;
	if (texture.m_internal_format == GL_RGB || texture.m_internal_format == GL_SRGB)
	{
		texture.m_internal_format = srgb ? GL_SRGB : GL_RGB;
	}

	if (texture.m_internal_format == GL_RGBA || texture.m_internal_format == GL_SRGB_ALPHA)
	{
		texture.m_internal_format = srgb ? GL_SRGB_ALPHA : GL_SRGB;
	}

	// make open-gl happy
	stbi_set_flip_vertically_on_load(true);

	int32_t width, height, number_of_components;
	const auto data = stbi_load(path.c_str(), &width, &height, &number_of_components, 0);

	if (data)
	{
		GLenum format_{};
		switch(number_of_components)
		{
		case 1:
			format_ = GL_RED;
			break;
		case 3:
			format_ = GL_RGB;
			break;
		case 4:
			format_ = GL_RGBA;
			break;
		default: 
			//do nothing;
			break;
		}

		if (target == GL_TEXTURE_1D)
		{
			texture.generate(width, texture.m_internal_format, format_, GL_UNSIGNED_BYTE, data);
		}
		else if (target == GL_TEXTURE_2D)
		{
			texture.generate(width,height, texture.m_internal_format, format_, GL_UNSIGNED_BYTE, data);
		}
	}
	else
	{
		std::cerr << "Failed to load texture at: " << path << '\n';
	}
	stbi_image_free(data);

	return textures[name] = texture;
}

aech::texture_t& aech::resource_manager::get_texture(const std::string& name)
{
	return textures[name]; 
}


aech::texture_t& aech::resource_manager::load_hdr_texture(const std::string& name, const std::string& path)
{
	if (textures.find(name) != std::end(textures))
	{
		return textures[name];
	}

	texture_t texture{};
	texture.m_target = GL_TEXTURE_2D;
	texture.m_filter_min = GL_LINEAR;
	texture.m_mipmap = false;

	stbi_set_flip_vertically_on_load(true);

	if (stbi_is_hdr(path.c_str()))
	{
		int32_t width, height, number_of_components;
		const auto data = stbi_loadf(path.c_str(), &width, &height, &number_of_components, 0);

		if (data)
		{
			GLenum internal_format, format;
			if (number_of_components == 3)
			{
				internal_format = GL_RGB32F;
				format = GL_RGB;
			}
			else if (number_of_components == 4)
			{
				internal_format = GL_RGBA32F;
				format = GL_RGBA;
			}

			texture.generate(width, height, internal_format, format, GL_FLOAT, data);
			stbi_image_free(data);
		}

		texture.m_width = width;
		texture.m_height = height;
	}
	else
	{
		std::cerr << "Failed to load HDR texture at: " << path << '\n';
	}

	return textures[name] = texture;
}

aech::entity_t resource_manager::process_node(const aiNode* node, const aiScene* scene)
{

	//TODO: parse materials
	const auto entity = engine.create_entity();
	engine.add_component(entity,
		transform_t{}
	);
	const auto transform = &engine.get_component<transform_t>(entity);
	engine.add_component(entity,
		scene_node_t{ transform }
	);
	engine.add_component(entity,
		mesh_filter_t{}
	);

	auto scene_node = &engine.get_component<scene_node_t>(entity);

	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		const auto a_mesh = scene->mMeshes[node->mMeshes[i]];
		auto a_material = scene->mMaterials[a_mesh->mMaterialIndex];
		const auto mesh = parse_mesh(a_mesh, scene);
		auto &mesh_filter = engine.get_component<mesh_filter_t>(entity);
		if (node ->mNumMeshes == 1)
		{
			// TODO: create mesh filter component
			mesh_filter.mesh = mesh;
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
				scene_node_t{ child_transform }
			);

			engine.add_component(child_entity,
				mesh_filter_t{}
			);


			const auto child_scene_node = &engine.get_component<scene_node_t>(child_entity);
			auto &child_mesh_filter = engine.get_component<mesh_filter_t>(child_entity);
			child_mesh_filter.mesh = mesh;
			scene_node->add_child(child_scene_node);
		}
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		const auto child_id = process_node(node->mChildren[i], scene);
		const auto child_scene_node = &engine.get_component<scene_node_t>(child_id);
		scene_node->add_child(child_scene_node);
	}

	return entity;
}

entity_t resource_manager::load_mesh(const std::string& path)
{
	Assimp::Importer importer{};
	const auto* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace
	);

	if (!scene)
	{
		std::cerr << "Assimp failed to load scene at path: " << path << '\n';
		return invalid_entity_id;
	}

	return process_node(scene->mRootNode, scene);
}

// TODO: caching of meshes
mesh_t* resource_manager::parse_mesh(aiMesh* mesh, const aiScene* scene)
{
	const auto it = meshes.find(mesh);
	if (it != std::end(meshes))
	{
		return &meshes[mesh];
	}

	auto ret_val = &meshes[mesh];
	ret_val->
		m_positions.resize(mesh->mNumVertices);
	ret_val->
		m_normals.resize(mesh->mNumVertices);

	// TODO: ?????
	if (mesh->mNumUVComponents[0])
	{
		ret_val->
			m_uvs.resize(mesh->mNumVertices);
		ret_val->
			m_tangents.resize(mesh->mNumVertices);
		ret_val->
			m_bitangents.resize(mesh->mNumVertices);
	}

	ret_val->
		m_indices.resize(mesh->mNumFaces * 3);

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		ret_val->
			m_positions[i] = vec3_t{
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		};

		ret_val->
			m_normals[i] = vec3_t{
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		};

		if (mesh->mTextureCoords[0])
		{
			ret_val->
				m_uvs[i] = vec2_t{
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			};
		}

		if (mesh->mTangents)
		{
			ret_val->
				m_tangents[i] = vec3_t{
				mesh->mTangents[i].x,
				mesh->mTangents[i].y,
				mesh->mTangents[i].z
			};
			ret_val->
				m_bitangents[i] = vec3_t{
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
			ret_val->
				m_indices[i * 3 + j] = mesh->mFaces[i].mIndices[j];
		}
	}

	ret_val->
		commit(true);

	return ret_val;
}

texture_cube_t& resource_manager::load_texture_cube(const std::string& name,
	const std::string& top,
	const std::string& bottom,
	const std::string& left,
	const std::string& right,
	const std::string& front,
	const std::string& back)
{
	if (texture_cubes.find(name) != std::end(texture_cubes))
	{
		return texture_cubes[name];
	}

	texture_cube_t texture{};

	stbi_set_flip_vertically_on_load(false);

	std::vector<std::string> faces{ top, bottom, left, right, front, back };

	for (size_t i = 0; i < faces.size(); i++)
	{
		int32_t width, height, number_of_components;
		const auto data = stbi_load(faces[i].c_str(), &width, &height, &number_of_components, 0);
		if (data)
		{
			const auto format = number_of_components == 3 ? GL_RGB : GL_RGBA;
			texture.generate_face(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width, height, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cerr << "Cube texture at path: " << faces[i] << " failed to load\n";
			stbi_image_free(data);
		}
		if (texture.m_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}

	return texture_cubes[name] = texture;
}

texture_cube_t& resource_manager::load_texture_cube(const std::string& name, const std::string& folder)
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
