#ifndef WRAPPERS_EGL_HPP
#define WRAPPERS_EGL_HPP

#include <EGL/egl.h>
// undefine countof from vcos_types for glm
#undef countof

#include <stdexcept>
#include <string>

namespace Wrappers {
    class EGLError : public std::runtime_error {
        public:
        EGLError(std::string str);
    };

    class EGL {
        public:
        EGL(int width, int height);
        ~EGL();

        int GetWidth() const;
        int GetHeight() const;

        private:
        EGLDisplay _display;
        int _major;
        int _minor;

        EGLContext _context;
        EGLSurface _surface;

        int _width;
        int _height;
    };
} // namespace Wrappers

#endif