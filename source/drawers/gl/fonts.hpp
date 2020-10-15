#ifndef DRAWERS_GL_FONTS_HPP
#define DRAWERS_GL_FONTS_HPP

#include "drawers/gl/drawable.hpp"
#include "vertex.hpp"
#include "wrappers/shader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>

namespace Drawers::GL {
    class Fonts : public Drawable {
        public:
        Fonts(std::shared_ptr<Wrappers::Shader> shader, int width, int height, const char *text);
        ~Fonts();

        void Draw(float time) final;
        void SetWavy(bool wavy);
        void MoveTo(glm::vec2 to);

        private:
        void LoadText(const char *text);

        FT_Library _freetype;
        FT_Face _face;

        std::shared_ptr<Wrappers::Shader> _shader;
        GLuint _posLoc;
        GLuint _texLoc;

        const float _scaleX;
        const float _scaleY;

        int _fontSize;

        struct {
            std::unique_ptr<TextVertex[]> vertices;
            int vertexCount;
            int quadCount;
            GLuint vbo;

            GLuint texture;
            std::unique_ptr<uint8_t[]> textureBuffer;
            int textureWidth;
            int textureHeight;

            int charCount;
        } _text;

        bool _wavy;

        glm::vec2 _at;
    };
} // namespace Drawers::GL

#endif