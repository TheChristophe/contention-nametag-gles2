#ifndef DRAWERS_GL_FONTS_HPP
#define DRAWERS_GL_FONTS_HPP

#include "drawable.hpp"
#include "vertex.hpp"
#include "wrappers/shader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>

namespace Drawers {
    class TextString : public Drawable {
        public:
        TextString(std::shared_ptr<Wrappers::Shader> shader, int width, int height, const char *text);

        TextString(const TextString &) = delete;
        TextString(TextString &&)      = default;
        TextString &operator=(const TextString &) = delete;
        TextString &operator=(TextString &&) = default;
        virtual ~TextString()                = default;

        void Draw(float time) final;
        void SetWavy(bool wavy);
        void MoveTo(glm::vec2 to);

        private:
        void LoadText(const char *text);

        FT_Library _freetype{};
        FT_Face _face{};

        std::shared_ptr<Wrappers::Shader> _shader{};
        GLuint _posLoc{};
        GLuint _texLoc{};

        float _scaleX{ 1.f };
        float _scaleY{ 1.f };

        int _fontSize{ 24 };

        struct {
            std::unique_ptr<TextVertex[]> vertices{};
            int vertexCount{ 0 };
            int quadCount{ 0 };
            GLuint vbo{};

            int charCount{ 0 };
        } _text;

        struct {
            GLuint handle{};
            std::unique_ptr<uint8_t[]> buffer{};
            unsigned width{ 0 };
            unsigned height{ 0 };
        } _texture;

        struct {
            bool _wavy{ false };
        } _properties;

        glm::vec2 _at{ 0.f, 0.f };
    };
} // namespace Drawers

#endif