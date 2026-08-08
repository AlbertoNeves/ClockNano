#pragma once

#include <Arduino.h>

namespace ConfigEEPROM
{
    //==========================================================
    // Endereços EEPROM
    //==========================================================

    constexpr uint8_t ADDR_BRIGHTNESS = 0;

    //==========================================================
    // Valores padrão
    //==========================================================

    constexpr uint8_t DEFAULT_BRIGHTNESS = 1;

    //==========================================================
    // Interface
    //==========================================================

    uint8_t loadBrightness();

    void saveBrightness(uint8_t value);
}