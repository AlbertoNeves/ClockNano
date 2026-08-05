#include "Graphics.h"

#include <Canvas.h>

//==========================================================

void Graphics::drawPixel(
    Canvas& canvas,
    uint8_t x,
    uint8_t y,
    bool state)
{
    canvas.setPixel(x, y, state);
}

//==========================================================

void Graphics::drawHLine(
    Canvas& canvas,
    uint8_t x,
    uint8_t y,
    uint8_t length)
{
    if (y >= canvas.height())
        return;

    if (x >= canvas.width())
        return;

    if (length == 0)
        return;

    if (x + length > canvas.width())
        length = canvas.width() - x;

    for (uint8_t i = 0; i < length; i++)
    {
        canvas.setPixel(x + i, y);
    }
}