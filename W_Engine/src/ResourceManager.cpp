#include <W_Engine/ResourceManager.h>

#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <W_Engine/Log.h>
#include <W_Engine/Model.h>
#include "Macros.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

static const std::string& getResourcesPath()
{
    static std::string path;

    if (path.empty())
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        path = buffer;

        size_t lastBackSlash = path.find_last_of("\\");
        if (lastBackSlash != std::string::npos)
        {
            path = path.substr(0, lastBackSlash);
        }
        path = path + "\\resources";
    }

    return path;
}

static W_Engine::Material loadMaterial(const aiMaterial& mat)
{
    W_Engine::Material material;
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat.Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.Diffuse = glm::vec3(color.r, color.b, color.g);

    mat.Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.Ambient = glm::vec3(color.r, color.b, color.g);

    mat.Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.Specular = glm::vec3(color.r, color.b, color.g);

    mat.Get(AI_MATKEY_SHININESS, shininess);
    material.Shininess = shininess;

    return material;
}

static std::vector<W_Engine::Texture> loadMaterialTextures(const aiMaterial& mat, aiTextureType type, W_Engine::TextureType textureType)
{
    std::vector<W_Engine::Texture> textures;
    for (unsigned int i = 0; i < mat.GetTextureCount(type); i++)
    {
        aiString str;
        mat.GetTexture(type, i, &str);
        W_Engine::Texture texture = W_Engine::ResourceManager::LoadTexture(str.C_Str(), textureType);
        textures.push_back(texture);
    }
    return textures;
}

static W_Engine::Mesh processMesh(aiMesh* mesh, const aiScene* scene)
{
    const int numVerts = mesh->mNumVertices;
    const int numIndicies = mesh->mNumFaces * 3;

    std::vector<W_Engine::Vertex> verticies(numVerts);
    std::vector<uint32_t> indicies(numIndicies);
    W_Engine::Texture texture;

    bool isTextureCoords = mesh->mTextureCoords[0];
    bool isVertexColors = mesh->mColors[0];

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

        if (isTextureCoords)
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

        if (isVertexColors)
        {
            vector.x = mesh->mColors[0][i].r;
            vector.y = mesh->mColors[0][i].g;
            vector.z = mesh->mColors[0][i].b;
            vertex.Color = vector;
        }
        else
        {
            vertex.Color = glm::vec3(1.f, 1.f, 1.f);
        }

        verticies[i] = vertex;
    }

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        ASSERT(face.mNumIndices == 3, "This renderer is only designed to use triangulated meshes");

        for (int j = 0; j < 3; j++)
        {
            indicies[i * 3 + j] = face.mIndices[j];
        }
    }

    std::vector<W_Engine::Texture> textures{};
    W_Engine::Material meshMaterial{};
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial& material = *scene->mMaterials[mesh->mMaterialIndex];

        auto diffuseTextures = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, W_Engine::TextureType::Diffuse);
        textures.reserve(textures.size() + diffuseTextures.size());
        std::move(diffuseTextures.begin(), diffuseTextures.end(), std::back_inserter(textures));

        auto specularTextures = loadMaterialTextures(material,
            aiTextureType_SPECULAR, W_Engine::TextureType::Specular);
        textures.reserve(textures.size() + specularTextures.size());
        std::move(specularTextures.begin(), specularTextures.end(), std::back_inserter(textures));

        meshMaterial = loadMaterial(material);
    }

    return W_Engine::Mesh(verticies, indicies, textures, meshMaterial);
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
    LOG_DEBUG("Meshes finished");

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, model);
    }
}

namespace W_Engine
{
    constexpr char SEPARATOR = '\\';

    Model ResourceManager::LoadModel(const std::string& filepath)
    {
        std::string path = getResourcesPath() + SEPARATOR + filepath;

        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error("File not found");
        }

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            path,
            aiProcess_Triangulate | aiProcess_FlipUVs
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG_ERROR("Error importing from %s: %s", filepath, importer.GetErrorString());
        }

        Model model{};
        model.Path = path;
        processNode(scene->mRootNode, scene, model);

        return model;
    }

    Shader ResourceManager::LoadShader(const std::string& filepath)
    {
        std::string path = getResourcesPath() + SEPARATOR + filepath;
        if (!std::filesystem::exists(path))
        {
            LOG_ERROR("Shader file not found at path: %s", path);
        }

        std::ifstream fileStream(path);
        std::string line;

        std::stringstream vShader{};
        std::stringstream fShader{};
        std::stringstream* currentShader = &vShader;

        int numShaders = 0;
        bool isError = false;

        while (!isError && std::getline(fileStream, line))
        {
            if (line.find("#version") != std::string::npos) 
            {
                ++numShaders; 
                switch (numShaders)
                {
                case 1:
                    //still reading vertex shader
                    break;
                case 2:
                    currentShader = &fShader;
                    break;
                case 3:
                    LOG_ERROR("Too many shader programs (identified with #version) declared in shader file %s", filepath);
                    isError = true;
                    break;
                }
            }
            *currentShader << line << '\n';
        }

        if (numShaders < 2)
        {
            LOG_ERROR("Shader program type (identified with #version) not found in shader file %s", filepath);
        }

        return Shader(vShader.str(), fShader.str());
    }

    //From LearnOpenGL
    Texture ResourceManager::LoadTexture(const std::string& filepath, TextureType textureType = TextureType::None)
    {
        return ResourceManager::LoadTexture(filepath.c_str(), textureType);
    }

    Texture ResourceManager::LoadTexture(const char* filepath, TextureType textureType = TextureType::None)
    {
        Texture texture{};
        texture.Type = textureType;
        glGenTextures(1, &texture.ID);
        glBindTexture(GL_TEXTURE_2D, texture.ID);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            LOG_ERROR("Failure to load texture at path %s", filepath);
        }
        stbi_image_free(data);
        return texture;
    }
}