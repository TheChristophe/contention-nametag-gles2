#ifndef DRAWABLE_GL_HPP
#define DRAWABLE_GL_HPP

namespace Drawers {
    class Drawable {
        public:
        virtual void Draw(float time) = 0;
    };
} // namespace Drawers

#endif