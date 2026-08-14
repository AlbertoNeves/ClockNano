#pragma once

#include <Arduino.h>

// Fonte de números 3x3 usada pelo indicador de segundos.
// Cada dígito ocupa exatamente 3 colunas x 3 linhas.
namespace Font3x3
{
    constexpr uint8_t Width = 3;
    constexpr uint8_t Height = 3;

    // Bits 0..2 = linhas 0..2.
    const uint8_t Digit0[Width] PROGMEM = {0x07, 0x05, 0x07};
    const uint8_t Digit1[Width] PROGMEM = {0x00, 0x07, 0x00};
    const uint8_t Digit2[Width] PROGMEM = {0x06, 0x05, 0x03};
    const uint8_t Digit3[Width] PROGMEM = {0x05, 0x05, 0x07};
    const uint8_t Digit4[Width] PROGMEM = {0x03, 0x02, 0x07};
    const uint8_t Digit5[Width] PROGMEM = {0x03, 0x05, 0x06};
    const uint8_t Digit6[Width] PROGMEM = {0x07, 0x05, 0x06};
    const uint8_t Digit7[Width] PROGMEM = {0x04, 0x06, 0x07};
    const uint8_t Digit8[Width] PROGMEM = {0x07, 0x05, 0x07};
    const uint8_t Digit9[Width] PROGMEM = {0x03, 0x05, 0x07};

    inline const uint8_t *glyph(char c)
    {
        switch (c)
        {
            case '0': return Digit0;
            case '1': return Digit1;
            case '2': return Digit2;
            case '3': return Digit3;
            case '4': return Digit4;
            case '5': return Digit5;
            case '6': return Digit6;
            case '7': return Digit7;
            case '8': return Digit8;
            case '9': return Digit9;
            default:  return nullptr;
        }
    }
}
