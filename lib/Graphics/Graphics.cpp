#include "Graphics.h"

#include <Canvas.h>
#include <Font.h>

#include <avr/pgmspace.h>

//
//==========================================================
//  PIXEL
//==========================================================
//

void Graphics::drawPixel(
    Canvas &canvas,
    uint8_t x,
    uint8_t y,
    bool state)
{
    canvas.setPixel(x, y, state);
}

//
//==========================================================
//  LINHA HORIZONTAL
//==========================================================
//

void Graphics::drawHLine(
    Canvas &canvas,
    uint8_t x,
    uint8_t y,
    uint8_t length)
{
    if (length == 0)
        return;

    if (x >= canvas.width())
        return;

    if (y >= canvas.height())
        return;

    if (x + length > canvas.width())
        length = canvas.width() - x;

    for (uint8_t i = 0; i < length; i++)
    {
        drawPixel(canvas, x + i, y);
    }
}

//
//==========================================================
//  LINHA VERTICAL
//==========================================================
//

void Graphics::drawVLine(
    Canvas &canvas,
    uint8_t x,
    uint8_t y,
    uint8_t length)
{
    if (length == 0)
        return;

    if (x >= canvas.width())
        return;

    if (y >= canvas.height())
        return;

    if (y + length > canvas.height())
        length = canvas.height() - y;

    for (uint8_t i = 0; i < length; i++)
    {
        drawPixel(canvas, x, y + i);
    }
}

//
//==========================================================
//  RETÂNGULO
//==========================================================
//

void Graphics::drawRectangle(
    Canvas &canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height)
{
    if (width == 0 || height == 0)
        return;

    drawHLine(canvas, x, y, width);

    drawHLine(canvas, x, y + height - 1, width);

    drawVLine(canvas, x, y, height);

    drawVLine(canvas, x + width - 1, y, height);
}

//
//==========================================================
//  RETÂNGULO PREENCHIDO
//==========================================================
//

void Graphics::fillRectangle(
    Canvas &canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height)
{
    if (width == 0 || height == 0)
        return;

    if (x >= canvas.width())
        return;

    if (y >= canvas.height())
        return;

    if (x + width > canvas.width())
        width = canvas.width() - x;

    if (y + height > canvas.height())
        height = canvas.height() - y;

    for (uint8_t row = 0; row < height; row++)
    {
        drawHLine(canvas, x, y + row, width);
    }
}

//
//==========================================================
//  BITMAP
//==========================================================
//

void Graphics::drawBitmap(
    Canvas &canvas,
    uint8_t x,
    uint8_t y,
    const uint8_t *bitmap,
    uint8_t width,
    uint8_t height,
    bool progmem)
{
    if (bitmap == nullptr)
        return;

    if (width == 0 || height == 0)
        return;

    for (uint8_t col = 0; col < width; col++)
    {
        uint8_t data;

        if (progmem)
            data = pgm_read_byte(bitmap + col);
        else
            data = bitmap[col];

        for (uint8_t row = 0; row < height; row++)
        {
            if (data & (1 << row))
            {
                drawPixel(
                    canvas,
                    x + col,
                    y + row);
            }
        }
    }
}

//
//==========================================================
//  CARACTERE
//==========================================================
//

void Graphics::drawChar(
    Canvas &canvas,
    uint8_t x,
    uint8_t y,
    char c)
{
    drawBitmap(
        canvas,
        x,
        y,
        Font::glyph(c),
        Font::width(),
        Font::height(),
        true);
}

//==========================================================
//  STRING
//==========================================================

void Graphics::drawString(
    Canvas &canvas,
    uint8_t x,
    uint8_t y,
    const char *text)
{
    if (text == nullptr)
        return;

    while (*text)
    {
        drawChar(
            canvas,
            x,
            y,
            *text++);

        x += charWidth();

        // Não adianta continuar se já saiu da tela
        if (x >= canvas.width())
            break;
    }
}
//==========================================================
//  CHAR WIDTH
//==========================================================

uint8_t Graphics::charWidth()
{
    return Font::width() + Font::spacing();
}
//==========================================================
//  TEXT WIDTH
//==========================================================

uint8_t Graphics::textWidth(const char *text)
{
    if (text == nullptr)
        return 0;

    uint8_t len = 0;

    while (*text++)
        len++;

    if (len == 0)
        return 0;

    return (len * Font::width()) +
           ((len - 1) * Font::spacing());
}
//==========================================================
//  CENTER X
//==========================================================

uint8_t Graphics::centerX(
    const Canvas &canvas,
    const char *text)
{
    uint8_t w = textWidth(text);

    if (w >= canvas.width())
        return 0;

    return (canvas.width() - w) / 2;
}
//==========================================================
//  STRING CENTERED
//==========================================================

void Graphics::drawStringCentered(
    Canvas &canvas,
    uint8_t y,
    const char *text)
{
    drawString(
        canvas,
        centerX(canvas, text),
        y,
        text);
}
