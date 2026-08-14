#pragma once

#include <Arduino.h>
#include <Font.h>

class Canvas;

namespace ClockView
{
    void setFont(ClockFontStyle style);
    ClockFontStyle font();

    void draw(
        Canvas& canvas,
        uint8_t hour,
        uint8_t minute,
        uint8_t second,
        bool showColon = true);
}
