#pragma once

#include <Arduino.h>

class Canvas;

namespace ClockView
{
    void draw(
        Canvas& canvas,
        uint8_t hour,
        uint8_t minute,
        bool showColon = true);
}