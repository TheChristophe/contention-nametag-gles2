
#include "triangle.hpp"

#include "vertex.hpp"

#include <GLES2/gl2.h>

#include <utility>

namespace Drawers {
    // x, y, z
    static const DecoVertex vertices[]{
        DecoVertex{ 2.0f, 0.0f, 0.f },    // cos/sin  0
        DecoVertex{ -1.f, 1.732f, 0.5f }, // cos/sin 2/3 pi
        DecoVertex{ -1.f, -1.732f, 1.0f } // cos/sin 4/3 pi
    };

    Triangle::Triangle(std::shared_ptr<Wrappers::Shader> shader)
        : _shader(std::move(shader))
        , _at(0, 0)
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

        _posLoc   = _shader->GetAttribLocation("pos");
        _luminLoc = _shader->GetAttribLocation("luminosity");
    }

    void Triangle::Draw(float time)
    {
        _shader->Use();
        _shader->Set("time", time * 4.f);
        _shader->Set("offset", _at);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glVertexAttribPointer(_posLoc, 2, GL_FLOAT, GL_FALSE, sizeof(DecoVertex), nullptr);
        glEnableVertexAttribArray(_posLoc);
        glVertexAttribPointer(_luminLoc, 1, GL_FLOAT, GL_FALSE, sizeof(DecoVertex), reinterpret_cast<void *>(sizeof(GLfloat) * 2));
        glEnableVertexAttribArray(_luminLoc);

        // render triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisableVertexAttribArray(_posLoc);
        glDisableVertexAttribArray(_luminLoc);
    }

    void Triangle::MoveTo(glm::vec2 to)
    {
        _at = to;
    }
} // namespace Drawers