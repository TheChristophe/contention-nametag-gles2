#ifndef WRAPPERS_OPENGL_HPP
#define WRAPPERS_OPENGL_HPP

#include "egl.hpp"

#include <cstdint>

namespace Wrappers {
    class OpenGL {
        public:
        OpenGL(int width, int height);

        void PreDraw();
        void PostDraw(uint8_t *outBuffer);

        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

        private:
        GLContextCreator _wrapper;
    };
} // namespace Wrappers

#endif