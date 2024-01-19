#include <W_Engine/ResourceManager.h>

#include <W_Engine/Application.h>

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

constexpr char SEPARATOR = '\\';

static const std::string getDirectoryFromPath(const std::string& path)
{
    std::string directory{};
    size_t lastBackslash = path.find_last_of(SEPARATOR);
    if (lastBackslash != std::string::npos)
    {
        directory = path.substr(0, lastBackslash);
    }
    return directory;
}

static const std::string& getResourcesPath()
{
    static std::string path;

    if (path.empty())
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        path = buffer;
        path = getDirectoryFromPath(path) + SEPARATOR + "resources";
    }

    return path;
}

static W_Engine::Material loadMaterial(const aiMaterial& mat)
{
    W_Engine::Material material;
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat.Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.Diffuse = glm::vec3(color.r, color.g, color.b);

    mat.Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.Ambient = glm::vec3(color.r, color.g, color.b);

    mat.Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.Specular = glm::vec3(color.r, color.g, color.b);

    return material;
}

static std::vector<std::unique_ptr<W_Engine::Texture>> loadMaterialTextures(
    const aiMaterial& mat,
    aiTextureType type,
    W_Engine::TextureType textureType,
    const std::string& modelPath
)
{
    std::vector<std::unique_ptr<W_Engine::Texture>> textures;
    for (unsigned int i = 0; i < mat.GetTextureCount(type); i++)
    {
        aiString str;
        mat.GetTexture(type, i, &str);
        std::unique_ptr<W_Engine::Texture> texture = 
            W_Engine::Application::Get().GetResourceManager().LoadTexture(
                modelPath + SEPARATOR + str.C_Str(),
                textureType
            );
        textures.push_back(std::move(texture));
    }
    return textures;
}

static W_Engine::Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& modelPath)
{
    const int numVerts = mesh->mNumVertices;
    const int numIndicies = mesh->mNumFaces * 3;

    std::vector<W_Engine::Vertex> verticies(numVerts);
    std::vector<uint32_t> indicies(numIndicies);

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

    std::vector<std::unique_ptr<W_Engine::Texture>> textures{};
    W_Engine::Material meshMaterial{};
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial& material = *scene->mMaterials[mesh->mMaterialIndex];

        auto diffuseTextures = loadMaterialTextures(
            material,
            aiTextureType_DIFFUSE, 
            W_Engine::TextureType::Diffuse,
            modelPath
        ); 
        
        textures.reserve(textures.size() + diffuseTextures.size());
        std::move(std::make_move_iterator(diffuseTextures.begin()), std::make_move_iterator(diffuseTextures.end()), std::back_inserter(textures));

        auto specularTextures = loadMaterialTextures(
            material,
            aiTextureType_SPECULAR,
            W_Engine::TextureType::Specular,
            modelPath
        );

        textures.reserve(textures.size() + specularTextures.size());
        std::move(std::make_move_iterator(specularTextures.begin()), std::make_move_iterator(specularTextures.end()), std::back_inserter(textures));

        meshMaterial = loadMaterial(material);
    }

    return W_Engine::Mesh(verticies, indicies, std::move(textures), meshMaterial);
}

static void processNode(aiNode* node, const aiScene* scene, W_Engine::Model& model)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model.Meshes.push_back(
            std::make_unique<W_Engine::Mesh>(
                processMesh(mesh, scene, model.Path)
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
    

    Model ResourceManager::LoadModel(const std::string& filepath)
    {
        std::string path = getResourcesPath() + SEPARATOR + filepath;

        if (!std::filesystem::exists(path))
        {
            LOG_ERROR("Model not found at path: %s", path);
            return Model();
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
        model.Path = getDirectoryFromPath(path);
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

    std::unique_ptr<Texture> ResourceManager::LoadTexture(const std::string& filepath, TextureType textureType = TextureType::None)
    {
        if (!std::filesystem::exists(filepath))
        {
            LOG_ERROR("Texture not found at path: %s", filepath);
        }
        return ResourceManager::LoadTexture(filepath.c_str(), textureType);
    }

    std::unique_ptr<Texture> ResourceManager::LoadTexture(const char* filepath, TextureType textureType = TextureType::None)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
        
        LOG_DEBUG("Width: %d, Height: %d, Channels: %d", width, height, nrChannels);

        std::unique_ptr<Texture> texture;
        if (data)
        {
            texture = std::make_unique<Texture>(textureType, data, width, height);
        }
        else
        {
            LOG_ERROR("Failure to load texture at path %s", filepath);
            texture = std::make_unique<Texture>(Texture::GeneratePlaceholderTexture());
        }

        stbi_image_free(data);


        return texture;
    }
}