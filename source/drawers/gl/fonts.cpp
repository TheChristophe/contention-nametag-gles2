#include "fonts.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

namespace Drawers::GL {
    Fonts::Fonts(std::shared_ptr<Wrappers::Shader> shader, int width, int height, const char *text)
        : _shader(shader)
        , _scaleX{ 2.f / width }
        , _scaleY{ 2.f / height }
        , _fontSize{ 24 }
        , _text{}
    {
        if (FT_Init_FreeType(&_freetype)) {
            throw std::runtime_error("could not load freetype");
        }

        if (FT_New_Face(_freetype, "ttf/DejaVuSans.ttf", 0, &_face)) {
            throw std::runtime_error("could not open font");
        }
        FT_Set_Pixel_Sizes(_face, 0, _fontSize);

        if (FT_Load_Char(_face, 'X', FT_LOAD_RENDER)) {
            throw std::runtime_error("could not load character");
        }

        glActiveTexture(GL_TEXTURE1);
        glGenTextures(1, &_text.texture);
        glBindTexture(GL_TEXTURE_2D, _text.texture);
        _shader->Use();
        _shader->Set("tex", 1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenBuffers(1, &_text.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _text.vbo);

        _posLoc = _shader->GetAttribLocation("pos");
        _texLoc = _shader->GetAttribLocation("texPos");

        //auto viewMatrix       = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0.0f, 1.0f, 0.0f));
        //auto projectionMatrix = glm::perspective(45.f, 2.f, 0.1f, 100.0f);
        //_shader.Set("projection", projectionMatrix * viewMatrix);

        LoadText(text);
    }

    Fonts::~Fonts()
    {
    }

    void Fonts::LoadText(const char *text)
    {
        constexpr bool lowRes{ 1 };

        // height above the baseline
        int glyphHeight{};
        // height below the baseline
        int glyphLowth{};

        int charCount{};

        for (const char *p = text; *p; p++) {
            if (FT_Load_Char(_face, *p, FT_LOAD_RENDER)) {
                throw std::runtime_error(std::string("failed to load ") + text + std::string(" due to '") + *p + std::string("'"));
            }

            auto &glyph = _face->glyph;

            // bitmap width minus horizontal offset
            _text.textureWidth += glyph->bitmap.width;
            //
            glyphHeight = std::max(glyphHeight, glyph->bitmap_top);
            glyphLowth  = std::max(glyphLowth, static_cast<int>(glyph->bitmap.rows - glyph->bitmap_top));

            charCount++;
        }

        // create string texture buffer
        _text.textureHeight = glyphHeight + glyphLowth;
        _text.textureBuffer = new uint8_t[_text.textureWidth * _text.textureHeight];
        memset(_text.textureBuffer, 0, _text.textureWidth * _text.textureHeight);

        _text.charCount = charCount;

        if constexpr (lowRes) {
            _text.quadCount   = charCount;
            _text.vertexCount = charCount * 4;
        }
        else {
            _text.quadCount   = 100;
            _text.vertexCount = 100 * 4;
        }

        _text.vertices = new TextVertex[_text.vertexCount];

        // current x coordinate in texture buffer
        int xOffset{};

        int i{};
        for (const char *p = text; *p; p++, i++) {
            // reload char bitmap
            FT_Load_Char(_face, *p, FT_LOAD_RENDER);

            auto &glyph = _face->glyph;

            // vertical offset for characters that are not full height
            const int yOffset = glyphHeight - glyph->bitmap_top;

            GLfloat xPos = static_cast<float>(xOffset) / _text.textureWidth;

            if constexpr (lowRes) {
                // left vertices
                _text.vertices[i * 4]     = TextVertex{ xPos, 1, xPos, 1 };
                _text.vertices[i * 4 + 1] = TextVertex{ xPos, 0, xPos, 0 };
            }

            // copy bitmaps to texture
            for (unsigned y = 0; y < glyph->bitmap.rows; y++) {
                for (unsigned x = 0; x < glyph->bitmap.width; x++) {
                    // destination buffer offset
                    const int xCoord      = xOffset + x;
                    const int yCoord      = yOffset + y;
                    const int textureAddr = yCoord * _text.textureWidth + xCoord;

                    // origin buffer offset
                    const int letterPixValue = glyph->bitmap.buffer[y * glyph->bitmap.width + x];

                    // add instead of set to avoid issues with overlapping letter bounding boxes
                    _text.textureBuffer[textureAddr] += letterPixValue;
                }
            }

            xOffset += glyph->bitmap.width;

            xPos = static_cast<float>(xOffset) / _text.textureWidth;
            if constexpr (lowRes) {
                // right vertices
                _text.vertices[i * 4 + 2] = TextVertex{ xPos, 1, xPos, 1 };
                _text.vertices[i * 4 + 3] = TextVertex{ xPos, 0, xPos, 0 };
            }
        }

        if constexpr (lowRes == false) {
            for (i = 0; i < _text.quadCount; i++) {
                _text.vertices[i * 4]     = TextVertex{ static_cast<float>(i) / _text.quadCount, 1, static_cast<float>(i) / _text.quadCount, 1 };
                _text.vertices[i * 4 + 1] = TextVertex{ static_cast<float>(i) / _text.quadCount, 0, static_cast<float>(i) / _text.quadCount, 0 };
                _text.vertices[i * 4 + 2] = TextVertex{ static_cast<float>(i + 1) / _text.quadCount, 1, static_cast<float>(i + 1) / _text.quadCount, 1 };
                _text.vertices[i * 4 + 3] = TextVertex{ static_cast<float>(i + 1) / _text.quadCount, 0, static_cast<float>(i + 1) / _text.quadCount, 0 };
            }
        }

        // load vertices to GL buffer
        glBindBuffer(GL_ARRAY_BUFFER, _text.vbo);
        glBufferData(GL_ARRAY_BUFFER, _text.vertexCount * sizeof(TextVertex), _text.vertices, GL_STATIC_DRAW);

        // create texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, _text.textureWidth, _text.textureHeight, 0,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, _text.textureBuffer);
    }

    void Fonts::Draw(float time)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _text.vbo);
        glVertexAttribPointer(_posLoc, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), nullptr);
        glEnableVertexAttribArray(_posLoc);
        glVertexAttribPointer(_texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), reinterpret_cast<void *>(sizeof(GLfloat) * 2));
        glEnableVertexAttribArray(_texLoc);

        glBindTexture(GL_TEXTURE_2D, _text.texture);

        _shader->Use();
        _shader->Set("time", time);

        for (int i = 0; i < _text.quadCount; i++) {
            glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
        }

        glDisableVertexAttribArray(_posLoc);
        glDisableVertexAttribArray(_texLoc);
    }
} // namespace Drawers::GL