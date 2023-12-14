#include <W_Engine/ResourceManager.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <W_Engine/Log.h>
#include "Macros.h"

static W_Engine::Mesh processMesh(aiMesh* mesh, const aiScene* scene)
{
	const int numVerts = mesh->mNumVertices;
	const int numIndicies = mesh->mNumFaces * 3;

	std::vector<W_Engine::Vertex> verticies(numVerts);
	std::vector<uint32_t> indicies(numIndicies);
	W_Engine::Texture texture;

	for (int i = 0; i < numVerts; i++)
	{
		W_Engine::Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec2;
			vec2.x = mesh->mTextureCoords[0][i].x;
			vec2.y = mesh->mTextureCoords[0][i].y;
			vertex.UVCoord = vec2;
		}
		else
		{
			vertex.UVCoord = glm::vec2{};
		}

		if (mesh->mColors[0])
		{
			glm::vec4 vec4;
			vec4.x = mesh->mColors[0][i].r;
			vec4.y = mesh->mColors[0][i].g;
			vec4.z = mesh->mColors[0][i].b;
			vec4.w = mesh->mColors[0][i].a;
			vertex.Color = vec4;
		}
		else
		{
			vertex.Color = glm::vec4(255, 0, 255, 255);
		}

		verticies[i] = vertex;
	}

	for (int i = 0; i < numIndicies; i++)
	{
		aiFace face = mesh->mFaces[i];
		ASSERT(face.mNumIndices == 3, "This renderer is only designed to use triangulated meshes");
		for (int j = 0; j < 3; j++)
		{
			indicies[i * 3 + j] = face.mIndices[j];
		}
	}
	return W_Engine::Mesh()
}

static void processNode(aiNode* node, const aiScene* scene, W_Engine::Model& model)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model.Meshes.push_back(
			std::make_unique<W_Engine::Mesh>(
				processMesh(mesh, scene)
			)
		);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, model);
	}
}

namespace W_Engine
{
	static const std::string RESOURCES_FILEPATH = ".resources/";

	Model ResourceManager::LoadModel(const std::string& filepath)
	{
		Assimp::Importer importer;
		std::string path = RESOURCES_FILEPATH + filepath;
		const aiScene* scene = importer.ReadFile(
			path,
			aiProcess_Triangulate | aiProcess_FlipUVs
		);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERROR("Error importing from %s: %s", filepath, importer.GetErrorString());
		}

		Model model{};
		processNode(scene->mRootNode, scene, model);

		return model;
	}
}