#include <W_Engine/Texture.h>

#include <glad/glad.h>

namespace W_Engine
{
    Texture Texture::GeneratePlaceholderTexture()
    {
        unsigned char whitePixel[4] = { 0,0,255,255 };
        return Texture(TextureType::None, whitePixel, 1, 1);
    }

    Texture::Texture(TextureType textureType, unsigned char* data, int width, int height)
    {
        m_type = textureType;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, m_id);
        /*GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            printf("OpenGL error %08x\n", err);
            abort();
        }*/
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_id);
    }
}