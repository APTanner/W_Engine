#pragma once 

#include "VertexArray.h"
#include "Shader.h"
#include "Transform.h"

#include <vector>

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
		glm::vec4 Color;
	};

	enum class TextureType
	{
		None,
		Specular,
		Diffuse
	};

	struct Texture
	{
		uint32_t ID;
		TextureType Type = TextureType::None;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& verticies, const std::vector<uint32_t> indicies, const Texture& texture = Texture{});

		void CreateMesh();
		void Draw(const Transform& transform, Shader& shader) const;

		//TODO figure out the most efficient / straightforward way of updating buffer data
		void SetVerticies(const std::vector<Vertex>& verticies);
		void SetIndicies(const std::vector<uint32_t>& indicies);
		void SetTexture(const Texture& texture);
	private:
		std::vector<Vertex> m_verticies;
		std::vector<uint32_t> m_indicies;
		Texture m_texture;

		VertexArray m_vertexArray = VertexArray{};
		bool m_valid = false;
	};

	class Model
	{
	public:
		std::vector<std::unique_ptr<Mesh>> Meshes;
	private:

	};
}