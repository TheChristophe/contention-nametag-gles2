
#include "openGL.hpp"
#include "egl.hpp"

#include <GLES2/gl2.h>

#include <cstdio>
#include <stdexcept>

namespace Wrappers {
    OpenGL::OpenGL(int width, int height)
        : _wrapper(width, height)
    {
        glViewport(0, 0, _wrapper.GetWidth(), _wrapper.GetHeight());

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        printf("GL Viewport size: %ix%i\n", viewport[2], viewport[3]);

        // test if viewport size is valid
        if (_wrapper.GetWidth() != viewport[2] || _wrapper.GetHeight() != viewport[3]) {
            throw std::runtime_error("glViewport/glGetIntegerv gives unexpected values");
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGL::PreDraw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /**
     * @brief Draw shape to buffer
     * 
     * @param buffer buffer of size 3 * width * height bytes
     * 
     * The 3 is due to needing R, G and B channels
     */
    void OpenGL::PostDraw(uint8_t *outBuffer)
    {
        // copy to out buffer
        glReadPixels(0, 0, _wrapper.GetWidth(), _wrapper.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, outBuffer);
    }

    int OpenGL::GetWidth() const
    {
        return _wrapper.GetWidth();
    }

    int OpenGL::GetHeight() const
    {
        return _wrapper.GetHeight();
    }
} // namespace Wrappers