#pragma once

#include <Arduino.h>

class Canvas;

class Display
{
public:

    bool begin();

    void update(const Canvas& canvas);

    void clear();

    void setBrightness(uint8_t value);

private:

    uint8_t m_brightness = 3;
};

extern Display display;