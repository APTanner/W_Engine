#include <W_Engine/Model.h>
#include <W_Engine/Log.h>

#include <W_Engine/Renderer.h>

namespace W_Engine
{
	Mesh::Mesh(const std::vector<Vertex>& verticies, const std::vector<uint32_t> indicies, const Texture& texture)
	{
		m_verticies = verticies;
		m_indicies = indicies;
		m_texture = texture;

		CreateMesh();
	}

	void Mesh::CreateMesh()
	{
		BufferLayout layout = {
			{ BufferDataType::Vector3, "position" },
			{ BufferDataType::Vector3, "normal" },
			{ BufferDataType::Vector2, "uv" },
			{ BufferDataType::Vector3, "color" }
		};

		m_vertexArray.Bind();

		m_vertexArray.SetVertexBuffer(
			std::make_unique<VertexBuffer>(
				reinterpret_cast<float*>(m_verticies.data()),
				sizeof(m_verticies.data()),
				layout
			)
		);

		m_vertexArray.SetElementBuffer(
			std::make_unique<ElementBuffer>(
				m_indicies.data(),
				sizeof(m_indicies.data())
			)
		);

		m_vertexArray.Unbind();
		m_valid = true;
	}

	void Mesh::Draw(const Transform& transform, Shader& shader) const
	{
		if (m_texture.Type != TextureType::None)
		{
			glBindTexture(GL_TEXTURE_2D, m_texture.ID);
			glActiveTexture(GL_TEXTURE0);
		}
		Renderer::Render(m_vertexArray, transform, shader);
	}

	//TODO
	void Mesh::SetVerticies(const std::vector<Vertex>& verticies)
	{
		m_verticies = verticies;
		m_valid = false;
	}
	void Mesh::SetIndicies(const std::vector<uint32_t>& indicies)
	{
		m_indicies = indicies;
		m_valid = false;
	}
	void Mesh::SetTexture(const Texture& texture)
	{
		m_texture = texture;
		m_valid = false
	}
}