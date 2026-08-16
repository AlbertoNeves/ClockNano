#pragma once

#include <Arduino.h>
#include <Font.h>

namespace ConfigEEPROM
{
    constexpr uint8_t ADDR_BRIGHTNESS = 0;
    constexpr uint8_t ADDR_FONT       = 1;
    constexpr uint8_t ADDR_MELODY_ONCE = 2;
    constexpr uint8_t ADDR_MELODY_DAILY = 3;
    constexpr uint8_t ADDR_MELODY_WEEKDAYS = 4;

    constexpr uint8_t DEFAULT_BRIGHTNESS = 1;
    constexpr ClockFontStyle DEFAULT_FONT = ClockFontStyle::Normal;

    uint8_t loadBrightness();
    void saveBrightness(uint8_t value);

    ClockFontStyle loadFont();
    void saveFont(ClockFontStyle value);

    uint8_t loadMelody(uint8_t category);
    void saveMelody(uint8_t category, uint8_t melody);
}
