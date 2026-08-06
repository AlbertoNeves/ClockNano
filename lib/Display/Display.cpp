#include "Display.h"

#include <Arduino.h>

#include <Canvas.h>
#include <Config.h>

#include <MD_MAX72xx.h>

namespace
{
    MD_MAX72XX mx(
        Config::Hardware,
        Config::PinDIN,
        Config::PinCLK,
        Config::PinCS,
        Config::DisplayModules);
}

Display display;

//==========================================================

Display::Display() : m_brightness(3)
{
}

//==========================================================

bool Display::begin()
{
    mx.begin();

    mx.control(
        MD_MAX72XX::INTENSITY,
        m_brightness);

    mx.clear();

    mx.update();

    return true;
}

//==========================================================

void Display::clear()
{
    mx.clear();

    mx.update();
}

//==========================================================

void Display::setBrightness(uint8_t level)
{
    if (level > 15)
        level = 15;

    m_brightness = level;

    mx.control(
        MD_MAX72XX::INTENSITY,
        level);
}

//==========================================================

uint8_t Display::brightness() const
{
    return m_brightness;
}

//==========================================================

void Display::refresh(const Canvas& canvas)
{
    const uint8_t* frame = canvas.frameBuffer();

    for (uint8_t x = 0; x < canvas.width(); x++)
    {
        uint8_t column = frame[x];

        for (uint8_t y = 0; y < canvas.height(); y++)
        {
            bool pixel = column & (1 << y);

            mx.setPoint(
                y,
                31 - x,
                pixel);
        }
    }

    mx.update();
}