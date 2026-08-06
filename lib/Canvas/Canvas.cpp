#include "Canvas.h"

#include <string.h>


Canvas::Canvas()
{
    clear();
}

void Canvas::clear()
{
    memset(m_buffer, 0x00, sizeof(m_buffer));
}

void Canvas::fill()
{
    memset(m_buffer, 0xFF, sizeof(m_buffer));
}

void Canvas::setPixel(
    uint8_t x,
    uint8_t y,
    bool state)
{
    if (x >= Width)
        return;

    if (y >= Height)
        return;

    uint8_t mask = (1 << y);

    if (state)
        m_buffer[x] |= mask;
    else
        m_buffer[x] &= ~mask;
}

void Canvas::togglePixel(
    uint8_t x,
    uint8_t y)
{
    if (x >= Width)
        return;

    if (y >= Height)
        return;

    m_buffer[x] ^= (1 << y);
}

bool Canvas::getPixel(
    uint8_t x,
    uint8_t y) const
{
    if (x >= Width)
        return false;

    if (y >= Height)
        return false;

    return (m_buffer[x] & (1 << y)) != 0;
}

void Canvas::clearColumn(
    uint8_t column)
{
    if (column >= Width)
        return;

    m_buffer[column] = 0;
}

void Canvas::setColumn(
    uint8_t column,
    uint8_t value)
{
    if (column >= Width)
        return;

    m_buffer[column] = value;
}

uint8_t Canvas::getColumn(
    uint8_t column) const
{
    if (column >= Width)
        return 0;

    return m_buffer[column];
}

uint8_t Canvas::width() const
{
    return Width;
}

uint8_t Canvas::height() const
{
    return Height;
}

const uint8_t* Canvas::frameBuffer() const
{
    return m_buffer;
}