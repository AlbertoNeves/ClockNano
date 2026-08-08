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

    // Desliga atualização automática.
    // As alterações serão enviadas ao MAX7219
    // somente quando mx.update() for chamado.
    mx.control(
        MD_MAX72XX::UPDATE,
        MD_MAX72XX::OFF);

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
        mx.setColumn(
            31 - x,
            frame[x]);
    }

    mx.update();
}