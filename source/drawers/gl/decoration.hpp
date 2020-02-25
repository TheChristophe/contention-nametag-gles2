#ifndef DRAWERS_GL_DECORATION_HPP
#define DRAWERS_GL_DECORATION_HPP

#include "wrappers/shader.hpp"

#include <memory>

namespace Drawers::GL {
    /**
     * @brief OpenGL drawing class for background pictures (triangles, ...).
     * 
     */
    class Decoration {
        public:
        Decoration(std::shared_ptr<Wrappers::Shader> shader, int width, int height);

        void DrawTriangle(float time);

        private:
        std::shared_ptr<Wrappers::Shader> _shader;

        GLuint _vbo;

        GLuint _posLoc;
        GLuint _luminLoc;

        GLuint _bayerTexture;

        int _width;
        int _height;
    };
} // namespace Drawers::GL

#endif
