#ifndef WRAPPERS_EGL_HPP
#define WRAPPERS_EGL_HPP

#ifdef DEV_MODE
#include <SDL2/SDL_video.h>
#else
#include <EGL/egl.h>
// undefine countof from vcos_types for glm
#undef countof
#endif

#include <stdexcept>
#include <string>

namespace Wrappers {
    class EGLError : public std::runtime_error {
        public:
        explicit EGLError(std::string str);
    };

    class GLContextCreator {
        public:
        GLContextCreator(int width, int height);
        ~GLContextCreator();

        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

#ifdef DEV_MODE
        SDL_Window *GetWindow();
#endif

        private:
        int _width;
        int _height;

#ifdef DEV_MODE
        SDL_Window *_window;
        SDL_GLContext _context;
#else
        EGLDisplay _display;
        int _major;
        int _minor;

        EGLContext _context;
        EGLSurface _surface;

#endif
    };
} // namespace Wrappers

#endif