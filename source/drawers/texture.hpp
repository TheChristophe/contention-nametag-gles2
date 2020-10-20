#ifndef VIDEO_TEXTURE_H
#define VIDEO_TEXTURE_H

#include <glm/glm.hpp> // glm types

#include <GLES2/gl2.h>

#include <filesystem>

namespace Drawers {
    class Texture {
        public:
        explicit Texture(const std::filesystem::path &file, const bool &rgba = false, bool nearestNeighbour = false);
        Texture(const Texture &) = delete;
        ~Texture();

        /**
         * @brief Set the current opengl texture to this
         */
        void Use();

        /**
         * @brief Get texture size in pixels
         *
         * @return Vec2i texture size
         */
        [[nodiscard]] glm::vec2 GetSize() const;

        private:
        // opengl texture handle
        GLuint _texture;
        // texture size in pixels
        glm::vec2 _size;
    };
} // namespace Drawers

#endif