#include "egl.hpp"

namespace Wrappers {
    static const char *eglGetErrorStr()
    {
        switch (eglGetError()) {
            case EGL_SUCCESS: {
                return "The last function succeeded without error.";
            } break;
            case EGL_NOT_INITIALIZED: {
                return "EGL is not initialized, or could not be initialized, for the "
                       "specified EGL display connection.";
            } break;
            case EGL_BAD_ACCESS: {
                return "EGL cannot access a requested resource (for example a context "
                       "is bound in another thread).";
            } break;
            case EGL_BAD_ALLOC: {
                return "EGL failed to allocate resources for the requested operation.";
            } break;
            case EGL_BAD_ATTRIBUTE: {
                return "An unrecognized attribute or attribute value was passed in the "
                       "attribute list.";
            } break;
            case EGL_BAD_CONTEXT: {
                return "An EGLContext argument does not name a valid EGL rendering "
                       "context.";
            } break;
            case EGL_BAD_CONFIG: {
                return "An EGLConfig argument does not name a valid EGL frame buffer "
                       "configuration.";
            } break;
            case EGL_BAD_CURRENT_SURFACE: {
                return "The current surface of the calling thread is a window, pixel "
                       "buffer or pixmap that is no longer valid.";
            } break;
            case EGL_BAD_DISPLAY: {
                return "An EGLDisplay argument does not name a valid EGL display "
                       "connection.";
            } break;
            case EGL_BAD_SURFACE: {
                return "An EGLSurface argument does not name a valid surface (window, "
                       "pixel buffer or pixmap) configured for GL rendering.";
            } break;
            case EGL_BAD_MATCH: {
                return "Arguments are inconsistent (for example, a valid context "
                       "requires buffers not supplied by a valid surface).";
            } break;
            case EGL_BAD_PARAMETER: {
                return "One or more argument values are invalid.";
            } break;
            case EGL_BAD_NATIVE_PIXMAP: {
                return "A NativePixmapType argument does not refer to a valid native "
                       "pixmap.";
            } break;
            case EGL_BAD_NATIVE_WINDOW: {
                return "A NativeWindowType argument does not refer to a valid native "
                       "window.";
            } break;
            case EGL_CONTEXT_LOST: {
                return "A power management event has occurred. The application must "
                       "destroy all contexts and reinitialise OpenGL ES state and "
                       "objects to continue rendering.";
            } break;
            default: {

            } break;
        }
        return "Unknown error!";
    }

    EGLError::EGLError(std::string str)
        : std::runtime_error(str)
    {
    }

    EGL::EGL(int width, int height)
        : _width(width)
        , _height(height)
    {
        _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (_display == EGL_NO_DISPLAY) {
            throw EGLError(std::string("Failed to get EGL display! ") + eglGetErrorStr());
        }

        if (eglInitialize(_display, &_major, &_minor) == EGL_FALSE) {
            auto error = eglGetErrorStr();
            eglTerminate(_display);
            throw EGLError(std::string("Failed to get EGL version! ") + error);
        }

        printf("Initialized EGL version: %d.%d\n", _major, _minor);

        static const EGLint configAttribs[] = {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_BLUE_SIZE, 0, EGL_GREEN_SIZE, 0,
            EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 8,

            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
        };
        EGLint numConfigs;
        EGLConfig config;
        if (!eglChooseConfig(_display, configAttribs, &config, 1, &numConfigs)) {
            auto error = eglGetErrorStr();
            eglTerminate(_display);
            throw EGLError(std::string("Failed to get EGL config! ") + error);
        }

        // Width and height of the desired framebuffer
        static const EGLint bufferAttributes[] = {
            EGL_WIDTH,
            _width,
            EGL_HEIGHT,
            _height,
            EGL_NONE,
        };
        _surface = eglCreatePbufferSurface(_display, config, bufferAttributes);
        if (_surface == EGL_NO_SURFACE) {
            auto error = eglGetErrorStr();
            eglTerminate(_display);
            throw EGLError(std::string("Failed to create EGL surface! ") + error);
        }

        eglBindAPI(EGL_OPENGL_API);

        static const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
        };
        _context = eglCreateContext(_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (_context == EGL_NO_CONTEXT) {
            auto error = eglGetErrorStr();
            eglDestroySurface(_display, _surface);
            eglTerminate(_display);
            throw EGLError(std::string("Failed to create EGL context! ") + error);
        }

        eglMakeCurrent(_display, _surface, _surface, _context);
    }

    EGL::~EGL()
    {
        eglDestroyContext(_display, _context);
        eglDestroySurface(_display, _surface);
        eglTerminate(_display);
    }

    int EGL::GetWidth() const
    {
        return _width;
    }

    int EGL::GetHeight() const
    {
        return _height;
    }
} // namespace Wrappers