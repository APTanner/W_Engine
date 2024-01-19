#pragma once

#include <memory>
#include "Model.h"

namespace W_Engine
{
    class ResourceManager
    {
        friend class Application;
    public:
        Model LoadModel(const std::string& filepath);
        Shader LoadShader(const std::string& filepath);
        std::unique_ptr<Texture> LoadTexture(const char* filepath, TextureType textureType);
        std::unique_ptr<Texture> LoadTexture(const std::string& filepath, TextureType textureType);
    private:
        ResourceManager() {}
        ~ResourceManager() {}
    };
}