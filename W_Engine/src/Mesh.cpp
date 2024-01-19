#include <W_Engine/Model.h>
#include <W_Engine/Log.h>

#include <W_Engine/Renderer.h>
#include <W_Engine/Application.h>

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
	Mesh::Mesh(
        const std::vector<Vertex>& verticies,
        const std::vector<uint32_t> indicies,
        std::vector<std::unique_ptr<Texture>>&& textures,
        const Material& material
    )
	{
		m_verticies = verticies;
		m_indicies = indicies;
        m_material = material;

        bool isMultipleOfSameType = false;
        uint32_t bitField = 0;
        for (std::unique_ptr<Texture>& texture : textures)
        {
            switch(texture->GetTextureType())
            {
            case TextureType::None:
                LOG_ERROR("Texture should not have a 'None' type");
                break;
            case TextureType::Specular:
                if (isUniqueTextureType(TextureType::Specular, bitField)) { m_textures.push_back(std::move(texture)); }
                else { isMultipleOfSameType = true; }
                break;
            case TextureType::Diffuse:
                if (isUniqueTextureType(TextureType::Diffuse, bitField)) { m_textures.push_back(std::move(texture)); }
                else { isMultipleOfSameType = true; }
                break;
            case TextureType::Normal:
                if (isUniqueTextureType(TextureType::Normal, bitField)) { m_textures.push_back(std::move(texture)); }
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

        std::vector<float> vertexData;
        vertexData.reserve(m_verticies.size() * (3 + 3 + 2 + 3));

        for (const Vertex& v : m_verticies)
        {
            vertexData.push_back(v.Position.x);
            vertexData.push_back(v.Position.y);
            vertexData.push_back(v.Position.z);

            vertexData.push_back(v.Normal.x);
            vertexData.push_back(v.Normal.y);
            vertexData.push_back(v.Normal.z);

            vertexData.push_back(v.UVCoord.x);
            vertexData.push_back(v.UVCoord.y);

            vertexData.push_back(v.Color.x);
            vertexData.push_back(v.Color.y);
            vertexData.push_back(v.Color.z);
        }

		m_vertexArray.SetVertexBuffer(
			std::make_unique<VertexBuffer>(
				vertexData.data(),
				sizeof(float) * vertexData.size(),
				layout
			)
		);

        m_vertexArray.SetElementBuffer(
            std::make_unique<ElementBuffer>(
                m_indicies.data(),
                sizeof(uint32_t) * m_indicies.size()
			)
		);
		m_valid = true;
	}

	void Mesh::Draw(const Transform& transform, Shader& shader) const
	{
        //set material
        shader.SetMaterial(m_material);

        //set textures

        //initialize all textures to be a 1x1 white pixel
        int j = 0;
        for (const auto& pair : TEXTURE_SAMPLER_NAMES)
        {
            glActiveTexture(GL_TEXTURE0 + j);
            auto a = pair.second;
            shader.SetInt(pair.second, j);
            glBindTexture(GL_TEXTURE_2D, Texture::GetPlaceHolderTexture().GetID());
            ++j;
        }

        //if the mesh has textures then replace the white pixels
        for (int i = 0; i < m_textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            auto a = TEXTURE_SAMPLER_NAMES.at(m_textures[i]->GetTextureType());
            shader.SetInt(TEXTURE_SAMPLER_NAMES.at(m_textures[i]->GetTextureType()), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i]->GetID());
            
        }

		Application::Get().GetRenderer().Render(m_vertexArray, transform, shader);
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