#pragma once

#include <Arduino.h>

// Fonte numérica 3x7 proveniente de fonts.h (dig3x7).
// Cada dígito ocupa 3 colunas e 7 linhas.
namespace Font3x7
{
constexpr uint8_t Width   = 3;
constexpr uint8_t Height  = 7;
constexpr uint8_t Spacing = 1;

// Os dados originais de dig3x7 usam os bits 1..7.
// Aqui os bytes já estão normalizados para bits 0..6,
// facilitando o uso pelo Canvas.
const uint8_t Digit0[Width] PROGMEM = {0x7F, 0x41, 0x7F};
const uint8_t Digit1[Width] PROGMEM = {0x04, 0x02, 0x7F};
const uint8_t Digit2[Width] PROGMEM = {0x79, 0x49, 0x4F};
const uint8_t Digit3[Width] PROGMEM = {0x41, 0x49, 0x7F};
const uint8_t Digit4[Width] PROGMEM = {0x1F, 0x10, 0x7E};
const uint8_t Digit5[Width] PROGMEM = {0x4F, 0x49, 0x79};
const uint8_t Digit6[Width] PROGMEM = {0x7F, 0x49, 0x79};
const uint8_t Digit7[Width] PROGMEM = {0x01, 0x71, 0x0F};
const uint8_t Digit8[Width] PROGMEM = {0x7F, 0x49, 0x7F};
const uint8_t Digit9[Width] PROGMEM = {0x4F, 0x49, 0x7F};

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
