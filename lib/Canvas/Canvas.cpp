#include "Canvas.h"

Canvas canvas;

void Canvas::begin()
{
    clear();
}

void Canvas::clear()
{
    memset(buffer,0,sizeof(buffer));
}

void Canvas::setPixel(uint8_t x,
                      uint8_t y,
                      bool state)
{
    if(x>=WIDTH) return;
    if(y>=HEIGHT) return;

    if(state)
        buffer[x] |= (1<<y);
    else
        buffer[x] &= ~(1<<y);
}

bool Canvas::getPixel(uint8_t x,
                      uint8_t y) const
{
    if(x>=WIDTH) return false;
    if(y>=HEIGHT) return false;

    return (buffer[x] & (1<<y));
}

void Canvas::invertPixel(uint8_t x,
                         uint8_t y)
{
    if(x>=WIDTH) return;
    if(y>=HEIGHT) return;

    buffer[x] ^= (1<<y);
}

uint8_t Canvas::getColumn(uint8_t x) const
{
    return buffer[x];
}