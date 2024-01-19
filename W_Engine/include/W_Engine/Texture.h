#pragma once

#include <unordered_map>
#include <string>
#include <memory>

namespace W_Engine
{
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

    class Texture
    {
    public:
        Texture(TextureType textureType, unsigned char* data, int width, int height);

        ~Texture();

        static const Texture& GetPlaceHolderTexture()
        {
            static Texture placeholder = GeneratePlaceholderTexture();
            return placeholder;
        }

        static Texture GeneratePlaceholderTexture();
        const uint32_t GetID() const { return m_id; }
        const TextureType GetTextureType() const { return m_type; }
    private:
        uint32_t m_id = 0;
        TextureType m_type = TextureType::None;
    private:
    };
}