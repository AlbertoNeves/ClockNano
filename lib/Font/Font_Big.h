#pragma once

#include <Arduino.h>

// Fonte GRANDE derivada da mybigfont original fornecida no projeto.
// A fonte original possui 10 caracteres com 20 bytes por dígito.
// Como o hardware disponível é 4 x 8x8 = 32x8, esta versão foi
// adaptada para 7x8 pixels por dígito, permitindo HH:MM no display.
namespace FontBig
{
constexpr uint8_t Width   = 7;
constexpr uint8_t Height  = 8;
constexpr uint8_t Spacing = 0;

const uint8_t Digit0[Width] PROGMEM = {0x7E,0xFF,0xC3,0xC3,0xC3,0xFF,0x7E};
const uint8_t Digit1[Width] PROGMEM = {0x00,0x18,0x38,0xFF,0xFF,0x00,0x00};
const uint8_t Digit2[Width] PROGMEM = {0xE6,0xF7,0xD3,0xC3,0xC3,0xCF,0xCE};
const uint8_t Digit3[Width] PROGMEM = {0x66,0xE7,0xC3,0xC3,0xC3,0xFF,0x7E};
const uint8_t Digit4[Width] PROGMEM = {0x1E,0x1E,0x36,0x66,0xFF,0xFF,0x06};
const uint8_t Digit5[Width] PROGMEM = {0xFF,0xFF,0xC6,0xC6,0xC6,0xC6,0xC6};
const uint8_t Digit6[Width] PROGMEM = {0x7E,0xFF,0xC6,0xC6,0xC6,0xCE,0x8C};
const uint8_t Digit7[Width] PROGMEM = {0xC0,0xC0,0xC0,0xC6,0xCF,0xFD,0xF8};
const uint8_t Digit8[Width] PROGMEM = {0x7E,0xFF,0xC3,0xC3,0xC3,0xFF,0x7E};
const uint8_t Digit9[Width] PROGMEM = {0x62,0xE3,0xC3,0xC3,0xC3,0xFF,0x7E};

const uint8_t Colon[1] PROGMEM = {0x66};

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
