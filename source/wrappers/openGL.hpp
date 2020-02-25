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

        int GetWidth() const;
        int GetHeight() const;

        private:
        EGL _wrapper;
    };
} // namespace Wrappers

#endif