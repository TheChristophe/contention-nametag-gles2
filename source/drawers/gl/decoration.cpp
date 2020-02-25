
#include "decoration.hpp"
#include "vertex.hpp"

#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>
#include <stdexcept>

namespace Drawers::GL {
    // x, y, z
    static const DecoVertex vertices[]{
        DecoVertex{ 2.0f, 0.0f, 0.f },     // cos/sin  0
        DecoVertex{ -1.f, 1.732f, 0.25f }, // cos/sin 2/3 pi
        DecoVertex{ -1.f, -1.732f, 0.5f }  // cos/sin 4/3 pi
    };

    Decoration::Decoration(std::shared_ptr<Wrappers::Shader> shader, int width, int height)
        : _shader(shader)
        , _width(width)
        , _height(height)
    {
        _shader->Use();

        // generate and setup vertex object
        glGenBuffers(1, &_vbo);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("failed to generate gl VBO");
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("failed to bind gl VBO");
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("failed to set gl VBO");
        }

        auto viewMatrix       = glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0, 0, 1), glm::vec3(0.0f, 1.0f, 0.0f));
        auto projectionMatrix = glm::perspective(45.f, static_cast<float>(_width) / _height, 0.1f, 100.0f);
        _shader->Set("projection", projectionMatrix * viewMatrix);
        _shader->Set("time", 0.f);
        _shader->Set("bayerTex", 0);

        uint8_t fourByFourBayer[4][4] = {
            { 0, 8, 2, 10 },
            { 12, 4, 14, 6 },
            { 3, 11, 1, 9 },
            { 15, 7, 13, 5 }
        };

        glGenTextures(1, &_bayerTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _bayerTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 4, 4, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, fourByFourBayer);

        _posLoc   = _shader->GetAttribLocation("pos");
        _luminLoc = _shader->GetAttribLocation("luminosity");
    }

    void Decoration::DrawTriangle(float time)
    {
        _shader->Use();
        _shader->Set("time", time * 4.f);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glVertexAttribPointer(_posLoc, 2, GL_FLOAT, GL_FALSE, sizeof(DecoVertex), nullptr);
        glEnableVertexAttribArray(_posLoc);
        glVertexAttribPointer(_luminLoc, 1, GL_FLOAT, GL_FALSE, sizeof(DecoVertex), reinterpret_cast<void *>(sizeof(GLfloat) * 2));
        glEnableVertexAttribArray(_luminLoc);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _bayerTexture);

        // render triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisableVertexAttribArray(_posLoc);
        glDisableVertexAttribArray(_luminLoc);
    }
} // namespace Drawers::GL