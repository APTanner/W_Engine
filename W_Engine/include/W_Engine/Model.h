#pragma once 

#include "VertexArray.h"
#include "Shader.h"
#include "Transform.h"

#include <vector>
#include <unordered_map>
#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace W_Engine
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 UVCoord;
        glm::vec3 Color;
    };

    enum class TextureType : int
    {
        None,
        Specular,
        Diffuse,
        Normal
    };

    const std::unordered_map<TextureType, std::string> TEXTURE_SAMPLER_NAMES{
        {TextureType::Specular, "t_specular"},
        {TextureType::Diffuse, "t_diffuse"},
        {TextureType::Normal, "t_normal"}
    };

    struct Material
    {
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        glm::vec3 Ambient;
        float Shininess ;
    };

    struct Texture
    {
        uint32_t ID = 0;
        TextureType Type = static_cast<TextureType>(-1);
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& verticies, const std::vector<uint32_t> indicies, const std::vector<Texture> textures, const Material& material);
        Mesh(Mesh&& other) noexcept :
            m_verticies(std::move(other.m_verticies)),
            m_indicies(std::move(other.m_indicies)),
            m_textures(std::move(other.m_textures)),
            m_material(std::move(other.m_material)),
            m_vertexArray(std::move(other.m_vertexArray)),
            m_valid(other.m_valid)
        {
        }

        void CreateMesh();
        void Draw(const Transform& transform, Shader& shader) const;

        //TODO figure out the most efficient / straightforward way of updating buffer data
        /*void SetVerticies(const std::vector<Vertex>& verticies);
        void SetIndicies(const std::vector<uint32_t>& indicies);
        void SetTextures(const Texture& texture);*/
    private:
        std::vector<Vertex> m_verticies;
        std::vector<uint32_t> m_indicies;
        std::vector<Texture> m_textures;
        Material m_material;

        VertexArray m_vertexArray = VertexArray{};
        bool m_valid = false;
    };

    class Model
    {
    public:
        std::vector<std::unique_ptr<Mesh>> Meshes;
        std::string Path;
    private:
    };
}