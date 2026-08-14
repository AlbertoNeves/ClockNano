#pragma once

#include <Arduino.h>

// Fonte DUPLA: dig6x8 proveniente de fonts.h.
// 6 colunas x 8 linhas por dígito.
namespace FontDual
{
constexpr uint8_t Width   = 6;
constexpr uint8_t Height  = 8;
constexpr uint8_t Spacing = 1;

const uint8_t Digit0[Width] PROGMEM = {0x7E, 0xFF, 0x81, 0x81, 0xFF, 0x7E};
const uint8_t Digit1[Width] PROGMEM = {0x00, 0x82, 0xFF, 0xFF, 0x80, 0x00};
const uint8_t Digit2[Width] PROGMEM = {0xC2, 0xE3, 0xB1, 0x99, 0x8F, 0x86};
const uint8_t Digit3[Width] PROGMEM = {0x42, 0xC3, 0x89, 0x89, 0xFF, 0x76};
const uint8_t Digit4[Width] PROGMEM = {0x38, 0x3C, 0x26, 0x23, 0xFF, 0xFF};
const uint8_t Digit5[Width] PROGMEM = {0x4F, 0xCF, 0x89, 0x89, 0xF9, 0x71};
const uint8_t Digit6[Width] PROGMEM = {0x7E, 0xFF, 0x89, 0x89, 0xFB, 0x72};
const uint8_t Digit7[Width] PROGMEM = {0x01, 0x01, 0xF1, 0xF9, 0x0F, 0x07};
const uint8_t Digit8[Width] PROGMEM = {0x76, 0xFF, 0x89, 0x89, 0xFF, 0x76};
const uint8_t Digit9[Width] PROGMEM = {0x4E, 0xDF, 0x91, 0x91, 0xFF, 0x7E};

// Dois pontos compactos para o relógio 32x8.
const uint8_t Colon[2] PROGMEM = {0x18, 0x18};

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
