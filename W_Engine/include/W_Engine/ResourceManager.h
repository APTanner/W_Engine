#pragma once

#include <memory>
#include <W_Engine/Model.h>

namespace W_Engine
{
    class ResourceManager
    {
    public:
        static Model LoadModel(const std::string& filepath);
        static Shader LoadShader(const std::string& filepath);
        static Texture LoadTexture(const char* filepath, TextureType textureType);
        static Texture LoadTexture(const std::string& filepath, TextureType textureType);
    };
}