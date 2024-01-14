#pragma once

#include <memory>
#include <W_Engine/Model.h>

namespace W_Engine
{
    class ResourceManager
    {
        friend class Application;
    public:
        Model LoadModel(const std::string& filepath);
        Shader LoadShader(const std::string& filepath);
        Texture LoadTexture(const char* filepath, TextureType textureType);
        Texture LoadTexture(const std::string& filepath, TextureType textureType);
    private:
        ResourceManager() {}
        ~ResourceManager() {}
    };
}