#ifndef DRAWERS_BUFFER_DRAWER_HPP
#define DRAWERS_BUFFER_DRAWER_HPP

#include "util/bitmaps.hpp"
#include "wrappers/driver.hpp"

namespace Drawers::Buffer {
    /**
     * @brief 2D Drawing class directly drawing onto framebuffer
     * 
     * Most of this code is leftover from (license-free) waveshare demo code.
     * It is encouraged to implement and use similar operations in OpenGL instead.
     */
    class Drawer {
        public:
        enum class DotSize {
            Square1x1 = 1,
            Square2x2,
            Square3x3,
            Square4x4,
            Square5x5,
            Square6x6,
            Square7x7,
            Square8x8,

            Default = Square1x1
        };

        enum class DotFill {
            Around = 1,
            RightUp,

            Default = Around
        };

        enum class LineStyle {
            Solid = 0,
            Dotted
        };

        enum class Fill {
            Empty = 0,
            Full
        };

        enum class Color {
            White = 0xFF,
            Black = 0x00,

            OledBackground = Black,
            FontBackground = Black,
            FontForeground = White,

        };

        Drawer(Wrappers::Driver &driver);

        void DrawPoint(int x, int y, Color color = Color::White, DotFill fill = DotFill::Default, DotSize size = DotSize::Default);
        void DrawLine(int x, int y, int xEnd, int yEnd, Color color = Color::White, LineStyle style = LineStyle::Solid, DotSize size = DotSize::Default);
        void DrawRectangle(int x, int y, int xEnd, int yEnd, Color color = Color::White, Fill fill = Fill::Full, DotSize size = DotSize::Default);
        void DrawCircle(int xCenter, int yCenter, int radius, Color color = Color::White, Fill fill = Fill::Full, DotSize size = DotSize::Default);
        void DrawBitmap(int x, int y, const uint8_t *bmp, int width, int height);
        void DrawBitmap(int x, int y, const Bitmap &bitmap);

        void ShowDemo();

        private:
        Wrappers::Driver &_driver;
        const Wrappers::Driver::State &_state;
    };
} // namespace Drawers::Buffer

#endif
