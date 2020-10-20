#ifndef DRAWERS_GL_DECORATION_HPP
#define DRAWERS_GL_DECORATION_HPP

#include "drawable.hpp"
#include "wrappers/shader.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace Drawers {
    /**
     * @brief OpenGL drawing class for background pictures (triangles, ...).
     * 
     */
    class Triangle : public Drawable {
        public:
        explicit Triangle(std::shared_ptr<Wrappers::Shader> shader);
        Triangle(const Triangle &) = delete;

        void Draw(float time) final;

        void MoveTo(glm::vec2 to);

        private:
        std::shared_ptr<Wrappers::Shader> _shader;

        GLuint _vbo;

        GLuint _posLoc;
        GLuint _luminLoc;

        glm::vec2 _at;
    };
} // namespace Drawers

#endif
