#ifndef DRAWERS_GL_VERTEX_HPP
#define DRAWERS_GL_VERTEX_HPP

#include <GLES2/gl2.h>

namespace Drawers::GL {
    struct DecoVertex {
        GLfloat x;
        GLfloat y;
        GLfloat luminosity;
    };

    struct TextVertex {
        GLfloat x;
        GLfloat y;
        GLfloat texX;
        GLfloat texY;
    };

    struct SpriteVertex {
        GLfloat x;
        GLfloat y;
        GLfloat texX;
        GLfloat texY;
    };
} // namespace Drawers::GL

#endif