#pragma once

#include <Arduino.h>
#include <Font.h>

namespace ConfigEEPROM
{
    constexpr uint8_t ADDR_BRIGHTNESS = 0;
    constexpr uint8_t ADDR_FONT       = 1;

    constexpr uint8_t DEFAULT_BRIGHTNESS = 1;
    constexpr ClockFontStyle DEFAULT_FONT = ClockFontStyle::Normal;

    uint8_t loadBrightness();
    void saveBrightness(uint8_t value);

    ClockFontStyle loadFont();
    void saveFont(ClockFontStyle value);
}
