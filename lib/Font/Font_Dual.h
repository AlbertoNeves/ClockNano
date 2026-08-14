#pragma once

#include <Arduino.h>

// Fonte DUPLA: versão compacta e mais pesada da Font5x7.
// Foi comprimida horizontalmente para caber HH:MM nos 32 pixels.
namespace FontDual
{
constexpr uint8_t Width   = 4;
constexpr uint8_t Height  = 7;
constexpr uint8_t Spacing = 1;

const uint8_t Digit0[Width] PROGMEM = {0x7F,0x59,0x4D,0x7F};
const uint8_t Digit1[Width] PROGMEM = {0x42,0x7F,0x7F,0x40};
const uint8_t Digit2[Width] PROGMEM = {0x73,0x59,0x49,0x4F};
const uint8_t Digit3[Width] PROGMEM = {0x63,0x49,0x49,0x7F};
const uint8_t Digit4[Width] PROGMEM = {0x1C,0x16,0x7F,0x7F};
const uint8_t Digit5[Width] PROGMEM = {0x6F,0x49,0x49,0x79};
const uint8_t Digit6[Width] PROGMEM = {0x7F,0x49,0x49,0x7B};
const uint8_t Digit7[Width] PROGMEM = {0x71,0x79,0x0D,0x07};
const uint8_t Digit8[Width] PROGMEM = {0x7F,0x49,0x49,0x7F};
const uint8_t Digit9[Width] PROGMEM = {0x6F,0x49,0x49,0x7F};
const uint8_t Colon[Width] PROGMEM = {0x00,0x36,0x36,0x00};

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
        case ':': return Colon;
        default:  return nullptr;
    }
}
}
