#include "sprite.hpp"

#include "texture.hpp" // Texture

#include <GLES2/gl2.h>

#include <cstring> // memcpy

static_assert(sizeof(GLfloat) == sizeof(float));
static_assert(sizeof(GLuint) == sizeof(uint32_t));

namespace Drawers::GL {

    Sprite::Sprite(std::shared_ptr<Wrappers::Shader> shader, std::filesystem::path file, bool transparent, glm::vec2 texCoord, glm::vec2 texSize)
        : _shader(shader)
        , _size(1.f, 1.f)
        , _pos(0.f, 0.f)
        , _scale(1.f, 1.f)
        , _texture(new Texture(file, transparent))
    {
        //glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("failed to generate gl VBO");
        }

        //glBindVertexArray(_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("failed to bind gl VBO");
        }

        if (texSize.x < 0.f) {
            texSize = _texture->GetSize();
        }

        // scale to texture size where 30.f is magical correction factor
        // TODO: magical correction factor???
        _scale = texSize / 29.f;

        // remember half pixel correction
        glm::vec2 textureFrom;
        textureFrom.x = static_cast<float>(texCoord.x + 0.5) / _texture->GetSize().x;
        textureFrom.y = 1 - static_cast<float>(texCoord.y + 0.5) / _texture->GetSize().y;

        glm::vec2 textureTo;
        textureTo.x = static_cast<float>((texCoord + texSize).x - 0.5) / _texture->GetSize().x;
        textureTo.y = 1 - static_cast<float>((texCoord + texSize).y - 0.5) / _texture->GetSize().y;

        SpriteVertex vertices[6] = {
            { 0.f, 0.f, textureFrom.x, textureFrom.y },
            { _size.x, 0.0f, textureTo.x, textureFrom.y },
            { _size.x, _size.y, textureTo.x, textureTo.y },
            { _size.x, _size.y, textureTo.x, textureTo.y },
            { 0.0f, _size.y, textureFrom.x, textureTo.y },
            { 0.0f, 0.0f, textureFrom.x, textureFrom.y }
        };

        memcpy(_vertices, vertices, sizeof(_vertices));

        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("failed to set gl VBO");
        }

        _posLoc = _shader->GetAttribLocation("pos");
        _texLoc = _shader->GetAttribLocation("texCoord");

        _shader->Set("texture1", 1);
    }

    Sprite::~Sprite()
    {
        //glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
    }

    //uint32_t Sprite::GetVAO() const
    //{
    //    return _vao;
    //}
    Texture *Sprite::GetTexture() const
    {
        return _texture;
    }

    glm::vec2 Sprite::GetSize() const
    {
        return glm::vec2(_scale.x * _size.x, _scale.y * _size.y);
    }
    glm::vec2 Sprite::GetBaseSize() const
    {
        return _size;
    }

    void Sprite::Draw(float time)
    {
        _shader->Use();
        _shader->Set("time", time * 4.f);
        _shader->Set("offset", _pos);
        _shader->Set("scale", _scale);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (2 + 2) * sizeof(float), reinterpret_cast<GLvoid *>(0));
        glEnableVertexAttribArray(_posLoc);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (2 + 2) * sizeof(float), reinterpret_cast<GLvoid *>(2 * sizeof(float)));
        glEnableVertexAttribArray(_texLoc);

        _texture->Use();

        // render triangle
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisableVertexAttribArray(_texLoc);
        glDisableVertexAttribArray(_posLoc);
    }

    void Sprite::MoveTo(glm::vec2 to)
    {
        _pos = to;
    }
} // namespace Drawers::GL