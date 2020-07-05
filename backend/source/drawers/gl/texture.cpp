#include "texture.hpp"

#include <stb_image.h> // to load image

#include <cstdio>

namespace Drawers::GL {

    const int mipmapLevels = 8;

    Texture::Texture(std::filesystem::path file, const bool &rgba, const bool nearestNeighbour)
    {
        *const_cast<bool *>(&rgba) = true;
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //float color[]{ 1.0f, 0.0f, 0.0f, 1.0f };
        //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

        // set texture filtering parameters
        if (nearestNeighbour) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        // load image, create texture and generate mipmaps
        int width{}, height{}, nrChannels{};
        unsigned char *data{ stbi_load(file.generic_string().c_str(), &width, &height, &nrChannels, (rgba ? STBI_rgb_alpha : STBI_rgb)) };
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, (rgba ? GL_RGBA : GL_RGB), width, height, 0, (rgba ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);

            /*if (nearestNeighbour) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevels - 1);
                glGenerateMipmap(GL_TEXTURE_2D);
            }*/
        }
        else {
            fprintf(stderr, "failed to load texture %s\n", file.generic_string().c_str());
        }
        stbi_image_free(data);

        _size.x = width;
        _size.y = height;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &_texture);
    }

    void Texture::Use()
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _texture);
    }

    glm::vec2 Texture::GetSize() const
    {
        return _size;
    }
} // namespace Drawers::GL