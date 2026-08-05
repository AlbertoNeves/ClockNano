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

//==========================================================

void Graphics::drawVLine(
    Canvas& canvas,
    uint8_t x,
    uint8_t y,
    uint8_t length)
{
    if (x >= canvas.width())
        return;

    if (y >= canvas.height())
        return;

    if (length == 0)
        return;

    if (y + length > canvas.height())
        length = canvas.height() - y;

    for (uint8_t i = 0; i < length; i++)
    {
        drawPixel(canvas, x, y + i);
    }
}
//==========================================================

void Graphics::drawRectangle(
    Canvas& canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height)
{
    if (width == 0 || height == 0)
        return;

    drawHLine(canvas, x, y, width);

    drawHLine(canvas,
              x,
              y + height - 1,
              width);

    drawVLine(canvas,
              x,
              y,
              height);

    drawVLine(canvas,
              x + width - 1,
              y,
              height);
}
//==========================================================

void Graphics::fillRectangle(
    Canvas& canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height)
{
    if (width == 0 || height == 0)
        return;

    // Limita à área do display
    if (x >= canvas.width() || y >= canvas.height())
        return;

    if (x + width > canvas.width())
        width = canvas.width() - x;

    if (y + height > canvas.height())
        height = canvas.height() - y;

    // Desenha uma linha horizontal para cada linha do retângulo
    for (uint8_t row = 0; row < height; row++)
    {
        drawHLine(
            canvas,
            x,
            y + row,
            width);
    }
}
