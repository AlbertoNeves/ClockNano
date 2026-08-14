#pragma once

#include <Arduino.h>

// Fonte dos números usada no modo SEGUNDOS.
// Baseada na dig5x8rn do fonts.h fornecido pelo usuário.
// 5 colunas x 8 linhas por dígito.
namespace FontRound
{
constexpr uint8_t Width  = 5;
constexpr uint8_t Height = 8;
constexpr uint8_t Spacing = 1;

const uint8_t Digit0[Width] PROGMEM = {0x7E, 0x81, 0x81, 0xFF, 0x7E};
const uint8_t Digit1[Width] PROGMEM = {0x04, 0x02, 0xFF, 0xFF, 0x00};
const uint8_t Digit2[Width] PROGMEM = {0xF1, 0x89, 0x89, 0x8F, 0x86};
const uint8_t Digit3[Width] PROGMEM = {0x81, 0x89, 0x89, 0xFF, 0x76};
const uint8_t Digit4[Width] PROGMEM = {0x1F, 0x10, 0x10, 0xFE, 0xFE};
const uint8_t Digit5[Width] PROGMEM = {0x8F, 0x89, 0x89, 0xF9, 0x71};
const uint8_t Digit6[Width] PROGMEM = {0x7E, 0x89, 0x89, 0xF9, 0x70};
const uint8_t Digit7[Width] PROGMEM = {0x01, 0xC1, 0xF1, 0x3F, 0x0F};
const uint8_t Digit8[Width] PROGMEM = {0x76, 0x89, 0x89, 0xFF, 0x76};
const uint8_t Digit9[Width] PROGMEM = {0x0E, 0x91, 0x91, 0xFF, 0x7E};

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
