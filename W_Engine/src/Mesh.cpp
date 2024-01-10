#include <W_Engine/Model.h>
#include <W_Engine/Log.h>

#include <W_Engine/Renderer.h>

#include <glad/glad.h>

static bool isUniqueTextureType(W_Engine::TextureType textureType, uint32_t& bitField)
{
    uint32_t bit = 1u << static_cast<int>(textureType);
    bool unique = (bitField & bit) == 0;
    bitField |= bit;
    return unique;
}

namespace W_Engine
{
	Mesh::Mesh(const std::vector<Vertex>& verticies, const std::vector<uint32_t> indicies, const std::vector<Texture> textures, const Material& material)
	{
		m_verticies = verticies;
		m_indicies = indicies;
        m_material = material;

        bool isMultipleOfSameType = false;
        uint32_t bitField = 0;
        for (const Texture& texture : textures)
        {
            switch(texture.Type)
            {
            case TextureType::None:
                LOG_ERROR("Texture should not have a 'None' type");
                break;
            case TextureType::Specular:
                if (isUniqueTextureType(TextureType::Specular, bitField)) { m_textures.push_back(texture); }
                else { isMultipleOfSameType = true; }
                break;
            case TextureType::Diffuse:
                if (isUniqueTextureType(TextureType::Diffuse, bitField)) { m_textures.push_back(texture); }
                else { isMultipleOfSameType = true; }
                break;
            case TextureType::Normal:
                if (isUniqueTextureType(TextureType::Normal, bitField)) { m_textures.push_back(texture); }
                else { isMultipleOfSameType = true; }
                break;
            }
        }
        if (isMultipleOfSameType)
        {
            LOG_ERROR("Mesh has multiple textures of the same type; some textures will not be displayed");
        }

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
        for (int i = 0; i < m_textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            shader.SetInt(TEXTURE_SAMPLER_NAMES.at(m_textures[i].Type), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i].ID);
        }
        glActiveTexture(GL_TEXTURE0);
		Renderer::Render(m_vertexArray, transform, shader);
	}

	//TODO
	/*void Mesh::SetVerticies(const std::vector<Vertex>& verticies)
	{
		m_verticies = verticies;
		m_valid = false;
	}
	void Mesh::SetIndicies(const std::vector<uint32_t>& indicies)
	{
		m_indicies = indicies;
		m_valid = false;
	}
	void Mesh::SetTextures(const Texture& texture)
	{
		m_texture = texture;
		m_valid = false
	}*/
}