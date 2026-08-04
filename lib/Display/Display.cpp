#include "Display.h"
#include "Canvas.h"

#include <MD_MAX72xx.h>

#include "Config.h"

MD_MAX72XX mx(
    Config::Hardware,
    Config::PinDIN,
    Config::PinCLK,
    Config::PinCS,
    Config::DisplayModules);

Display display;

bool Display::begin()
{
    mx.begin();

    mx.control(MD_MAX72XX::INTENSITY, m_brightness);

    mx.clear();

    return true;
}

void Display::clear()
{
    mx.clear();
}

void Display::setBrightness(uint8_t value)
{
    if(value > 15)
        value = 15;

    m_brightness = value;

    mx.control(MD_MAX72XX::INTENSITY, value);
}

void Display::update(const Canvas& canvas)
{
    const uint8_t* p = canvas.data();

    for(uint8_t i = 0; i < 32; i++)
        mx.setColumn(i, p[i]);

    mx.update();
}