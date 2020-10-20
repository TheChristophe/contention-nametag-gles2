#ifndef VIDEO_SPRITE_H
#define VIDEO_SPRITE_H

#include "drawable.hpp"
#include "vertex.hpp"
#include "wrappers/shader.hpp"

#include <filesystem>
#include <glm/glm.hpp> // glm types
#include <memory>

namespace Drawers {
    class Sprite : public Drawable {
        public:
        Sprite(std::shared_ptr<Wrappers::Shader> shader, const std::filesystem::path& file, bool transparent = false, glm::vec2 texCoord = glm::vec2(0, 0), glm::vec2 texSize = glm::vec2(-1, -1));
        Sprite(const Sprite &) = delete;
        virtual ~Sprite();

        //uint32_t GetVAO() const;
        [[nodiscard]] class Texture *GetTexture() const;
        [[nodiscard]] glm::vec2 GetSize() const;
        [[nodiscard]] glm::vec2 GetBaseSize() const;

        void Draw(float time) override;

        void MoveTo(glm::vec2 to);

        private:
        std::shared_ptr<Wrappers::Shader> _shader;

        glm::vec2 _size;

        glm::vec2 _pos;
        glm::vec2 _scale;

        // opengl vertices
        SpriteVertex _vertices[6];

        // opengl vertex array object handle
        //uint32_t _vao;
        // opengl vertex buffer object handle
        uint32_t _vbo;

        // reference to used texture
        class Texture *_texture;

        uint32_t _posLoc;
        uint32_t _texLoc;
    };
} // namespace Drawers
#endif