#include "drawer.hpp"
#include <algorithm>

namespace Drawers::Buffer {
    Drawer::Drawer(Wrappers::Driver &driver)
        : _driver(driver)
        , _state(driver.GetState())
    {
    }

    void Drawer::DrawPoint(int x, int y, Color color, DotFill fill, DotSize size)
    {
        if (x > _state.width || y > _state.width) {
            return;
        }

        auto dotSize = static_cast<int>(size);

        if (size == DotSize::Default) {
            for (uint16_t xOffset = 0; xOffset < 2 * dotSize - 1; xOffset++) {
                for (uint16_t yOffset = 0; yOffset < 2 * dotSize - 1; yOffset++) {
                    _driver.SetColor(x + xOffset - dotSize, y + yOffset - dotSize, static_cast<Wrappers::Driver::ColorT>(color));
                }
            }
        }
        else {
            for (uint16_t xOffset = 0; xOffset < dotSize; xOffset++) {
                for (uint16_t yOffset = 0; yOffset < dotSize; yOffset++) {
                    _driver.SetColor(x + xOffset - 1, y + yOffset - 1, static_cast<Wrappers::Driver::ColorT>(color));
                }
            }
        }
    }

    void Drawer::DrawLine(int xFrom, int yFrom, int xTo, int yTo, Color color, LineStyle style, DotSize size)
    {
        if (xFrom > _state.width || yFrom > _state.height || xTo > _state.width || yTo > _state.height) {
            return;
        }

        if (xFrom > xTo) {
            std::swap(xFrom, xTo);
        }
        if (yFrom > yTo) {
            std::swap(yFrom, yTo);
        }

        auto x     = xFrom;
        auto y     = yFrom;
        int deltaX = xTo - xFrom >= 0 ? xTo - xFrom : xFrom - xTo;
        int deltaY = yTo - yFrom <= 0 ? yTo - yFrom : yFrom - yTo;

        // Increment direction, 1 is positive, -1 is counter;
        int xDir = xFrom < xTo ? 1 : -1;
        int yDir = yFrom < yTo ? 1 : -1;

        //Cumulative error
        int Esp             = deltaX + deltaY;
        int Line_Style_Temp = 0;

        for (;;) {
            Line_Style_Temp++;
            //Painted dotted line, 2 point is really virtual
            if (style == LineStyle::Dotted && Line_Style_Temp % 3 == 0) {
                DrawPoint(x, y, Color::FontBackground, DotFill::Default, size);
                Line_Style_Temp = 0;
            }
            else {
                DrawPoint(x, y, color, DotFill::Default, size);
            }
            if (2 * Esp >= deltaY) {
                if (x == xTo) {
                    break;
                }
                Esp += deltaY;
                x += xDir;
            }
            if (2 * Esp <= deltaX) {
                if (y == yTo) {
                    break;
                }

                Esp += deltaX;
                y += yDir;
            }
        }
    }

    void Drawer::DrawRectangle(int xFrom, int yFrom, int xTo, int yTo, Color color, Fill fill, DotSize size)
    {
        if (xFrom > _state.width || yFrom > _state.height || xTo > _state.width || yTo > _state.height) {
            return;
        }

        if (xFrom > xTo) {
            std::swap(xFrom, xTo);
        }
        if (yFrom > yTo) {
            std::swap(yFrom, yTo);
        }

        if (fill == Fill::Full) {
            for (auto y = yFrom; y < yTo; y++) {
                DrawLine(xFrom, y, xTo, y, color, LineStyle::Solid, size);
            }
        }
        else {
            DrawLine(xFrom, yFrom, xTo, yFrom, color, LineStyle::Solid, size);
            DrawLine(xFrom, yFrom, xFrom, yTo, color, LineStyle::Solid, size);
            DrawLine(xTo, yTo, xTo, yFrom, color, LineStyle::Solid, size);
            DrawLine(xTo, yTo, xFrom, yTo, color, LineStyle::Solid, size);
        }
    }
    void Drawer::DrawCircle(int xCenter, int yCenter, int radius, Color color, Fill fill, DotSize size)
    {
        if (xCenter > _state.width || yCenter >= _state.height) {
            return;
        }

        //Draw a circle from(0, R) as a starting point
        int x = 0;
        int y = radius;

        //Cumulative error,judge the next point of the logo
        int Esp = 3 - (radius << 1);

        if (fill == Fill::Full) {
            while (x <= y) { //Realistic circles
                for (int sCountY = x; sCountY <= y; sCountY++) {
                    DrawPoint(xCenter + x, yCenter + sCountY, color); //1
                    DrawPoint(xCenter - x, yCenter + sCountY, color); //2
                    DrawPoint(xCenter - sCountY, yCenter + x, color); //3
                    DrawPoint(xCenter - sCountY, yCenter - x, color); //4
                    DrawPoint(xCenter - x, yCenter - sCountY, color); //5
                    DrawPoint(xCenter + x, yCenter - sCountY, color); //6
                    DrawPoint(xCenter + sCountY, yCenter - x, color); //7
                    DrawPoint(xCenter + sCountY, yCenter + x, color);
                }
                if (Esp < 0)
                    Esp += 4 * x + 6;
                else {
                    Esp += 10 + 4 * (x - y);
                    y--;
                }
                x++;
            }
        }
        else { //Draw a hollow circle
            while (x <= y) {
                DrawPoint(xCenter + x, yCenter + y, color, DotFill::Default, size); //1
                DrawPoint(xCenter - x, yCenter + y, color, DotFill::Default, size); //2
                DrawPoint(xCenter - y, yCenter + x, color, DotFill::Default, size); //3
                DrawPoint(xCenter - y, yCenter - x, color, DotFill::Default, size); //4
                DrawPoint(xCenter - x, yCenter - y, color, DotFill::Default, size); //5
                DrawPoint(xCenter + x, yCenter - y, color, DotFill::Default, size); //6
                DrawPoint(xCenter + y, yCenter - x, color, DotFill::Default, size); //7
                DrawPoint(xCenter + y, yCenter + x, color, DotFill::Default, size); //0

                if (Esp < 0)
                    Esp += 4 * x + 6;
                else {
                    Esp += 10 + 4 * (x - y);
                    y--;
                }
                x++;
            }
        }
    }

    void Drawer::DrawBitmap(int x, int y, const uint8_t *bmp, int width, int height)
    {
        int byteWidth = (width + 7) / 8;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                if (*(bmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                    DrawPoint(x + i, y + j, Color::White);
                }
            }
        }
    }

    void Drawer::DrawBitmap(int x, int y, const Bitmap &bitmap)
    {
        int byteWidth = (bitmap.width + 7) / 8;
        for (int j = 0; j < bitmap.height; j++) {
            for (int i = 0; i < bitmap.width; i++) {
                if (*(bitmap.buffer + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                    DrawPoint(x + i, y + j, Color::White);
                }
            }
        }
    }

    void Drawer::ShowDemo()
    {
        printf("Clearing screen...\n");
        _driver.Clear();
        _driver.Display();

        printf("Drawing lines...\n");
        DrawLine(0, 1, _state.width, 1, Color::White, LineStyle::Solid, DotSize::Square2x2);
        DrawLine(0, 5, _state.width, 5, Color::White, LineStyle::Dotted);
        DrawLine(0, _state.height - 5, _state.width, _state.height - 5, Color::White, LineStyle::Dotted);
        DrawLine(0, _state.height, _state.width, _state.height, Color::White, LineStyle::Solid, DotSize::Square2x2);

        printf("Drawing rectangles...\n");
        DrawRectangle(5, 7, _state.width - 5, _state.height - 7, Color::White, Fill::Empty);
        DrawRectangle(10, 10, _state.width - 10, 20, Color::White, Fill::Full);

        printf("Drawing circles...\n");
        DrawCircle(10, 30, 3, Color::White, Fill::Full);
        DrawCircle(10, 40, 3, Color::White, Fill::Empty);
        DrawCircle(10, 50, 3, Color::White, Fill::Full);
        DrawCircle(_state.width - 10, 30, 3, Color::White, Fill::Full);
        DrawCircle(_state.width - 10, 40, 3, Color::White, Fill::Empty);
        DrawCircle(_state.width - 10, 50, 3, Color::White, Fill::Full);

        printf("Displaying...\n");
        _driver.Display();
        Hardware::DelayMS(2000);
    }
} // namespace Drawers::Buffer