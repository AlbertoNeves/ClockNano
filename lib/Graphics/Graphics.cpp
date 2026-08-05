#include "Graphics.h"

#include <Canvas.h>

void Graphics::drawPixel(
    Canvas& canvas,
    uint8_t x,
    uint8_t y,
    bool state)
{
    canvas.setPixel(x, y, state);
}