#ifndef FONT_5X3_H
#define FONT_5X3_H

#include <Arduino.h>

namespace Font5x3
{

//==========================================================
// Dimensões
//==========================================================

constexpr uint8_t Width   = 3;
constexpr uint8_t Height  = 5;
constexpr uint8_t Spacing = 1;

//==========================================================
// Espaço
//==========================================================

static const uint8_t Space[Width] PROGMEM =
{
    0x00,
    0x00,
    0x00
};

//==========================================================
// Números
//==========================================================

static const uint8_t Digit0[Width] PROGMEM =
{
    0x1F, 0x11, 0x1F
};

static const uint8_t Digit1[Width] PROGMEM =
{
    0x00, 0x1F, 0x00
};

static const uint8_t Digit2[Width] PROGMEM =
{
    0x1D, 0x15, 0x17
};

static const uint8_t Digit3[Width] PROGMEM =
{
    0x11, 0x15, 0x1F
};

static const uint8_t Digit4[Width] PROGMEM =
{
    0x07, 0x04, 0x1F
};

static const uint8_t Digit5[Width] PROGMEM =
{
    0x17, 0x15, 0x1D
};

static const uint8_t Digit6[Width] PROGMEM =
{
    0x1F, 0x15, 0x1D
};

static const uint8_t Digit7[Width] PROGMEM =
{
    0x01, 0x01, 0x1F
};

static const uint8_t Digit8[Width] PROGMEM =
{
    0x1F, 0x15, 0x1F
};

static const uint8_t Digit9[Width] PROGMEM =
{
    0x17, 0x15, 0x1F
};

//==========================================================
// Letras utilizadas por BRILHO
//==========================================================

static const uint8_t B[Width] PROGMEM =
{
    0x1F, 0x15, 0x0A
};

static const uint8_t R[Width] PROGMEM =
{
    0x1F, 0x05, 0x1A
};

static const uint8_t I[Width] PROGMEM =
{
    0x11, 0x1F, 0x11
};

static const uint8_t L[Width] PROGMEM =
{
    0x1F, 0x10, 0x10
};

static const uint8_t H[Width] PROGMEM =
{
    0x1F, 0x04, 0x1F
};

static const uint8_t O[Width] PROGMEM =
{
    0x0E, 0x11, 0x0E
};

//==========================================================
// Glyph
//==========================================================

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

        case 'B': return B;
        case 'R': return R;
        case 'I': return I;
        case 'L': return L;
        case 'H': return H;
        case 'O': return O;

        case ' ':
        default:
            return Space;
    }
}

} // namespace Font5x3

#endif