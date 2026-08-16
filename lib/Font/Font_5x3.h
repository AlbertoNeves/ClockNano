#ifndef FONT_5X3_H
#define FONT_5X3_H

#include <Arduino.h>

namespace Font5x3
{

//==========================================================
// Dimensoes
//==========================================================

constexpr uint8_t Width   = 3;
constexpr uint8_t Height  = 5;
constexpr uint8_t Spacing = 1;

//==========================================================
// Espaco
//==========================================================

static const uint8_t Space[Width] PROGMEM =
{
    0x00,
    0x00,
    0x00
};

//==========================================================
// Numeros
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
// Alfabeto A-Z
//==========================================================

static const uint8_t A[Width] PROGMEM =
{
    0x1F, 0x05, 0x1F
};

static const uint8_t B[Width] PROGMEM =
{
    0x1F, 0x15, 0x0A
};

static const uint8_t C[Width] PROGMEM =
{
    0x1F, 0x11, 0x11
};

static const uint8_t D[Width] PROGMEM =
{
    0x1F, 0x11, 0x0E
};

static const uint8_t E[Width] PROGMEM =
{
    0x1F, 0x15, 0x11
};

static const uint8_t F[Width] PROGMEM =
{
    0x1F, 0x05, 0x01
};

static const uint8_t G[Width] PROGMEM =
{
    0x1F, 0x15, 0x1D
};

static const uint8_t H[Width] PROGMEM =
{
    0x1F, 0x04, 0x1F
};

static const uint8_t I[Width] PROGMEM =
{
    0x11, 0x1F, 0x11
};

static const uint8_t J[Width] PROGMEM =
{
    0x10, 0x11, 0x1F
};

static const uint8_t K[Width] PROGMEM =
{
    0x1F, 0x04, 0x1B
};

static const uint8_t L[Width] PROGMEM =
{
    0x1F, 0x10, 0x10
};

static const uint8_t M[Width] PROGMEM =
{
    0x1F, 0x0A, 0x1F
};

static const uint8_t N[Width] PROGMEM =
{
    0x1F, 0x02, 0x1F
};

static const uint8_t O[Width] PROGMEM =
{
    0x0E, 0x11, 0x0E
};

static const uint8_t P[Width] PROGMEM =
{
    0x1F, 0x05, 0x02
};

static const uint8_t Q[Width] PROGMEM =
{
    0x0E, 0x19, 0x1E
};

static const uint8_t R[Width] PROGMEM =
{
    0x1F, 0x05, 0x1A
};

static const uint8_t S[Width] PROGMEM =
{
    0x17, 0x15, 0x1D
};

static const uint8_t T[Width] PROGMEM =
{
    0x01, 0x1F, 0x01
};

static const uint8_t U[Width] PROGMEM =
{
    0x1F, 0x10, 0x1F
};

static const uint8_t V[Width] PROGMEM =
{
    0x0F, 0x10, 0x0F
};

static const uint8_t W[Width] PROGMEM =
{
    0x1F, 0x0E, 0x1F
};

static const uint8_t X[Width] PROGMEM =
{
    0x1B, 0x04, 0x1B
};

static const uint8_t Y[Width] PROGMEM =
{
    0x03, 0x1C, 0x03
};

static const uint8_t Z[Width] PROGMEM =
{
    0x19, 0x15, 0x13
};

//==========================================================
// Simbolos
//==========================================================

static const uint8_t Colon[Width] PROGMEM =
{
    0x00, 0x0A, 0x00
};

static const uint8_t Plus[Width] PROGMEM =
{
    0x04, 0x1F, 0x04
};

static const uint8_t Minus[Width] PROGMEM =
{
    0x04, 0x04, 0x04
};

static const uint8_t Slash[Width] PROGMEM =
{
    0x18, 0x06, 0x01
};

static const uint8_t Dot[Width] PROGMEM =
{
    0x10, 0x00, 0x00
};

//==========================================================
// Glyph
//==========================================================

inline const uint8_t *glyph(char c)
{
    switch (c)
    {
        // Numeros
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

        // Alfabeto
        case 'A': return A;
        case 'B': return B;
        case 'C': return C;
        case 'D': return D;
        case 'E': return E;
        case 'F': return F;
        case 'G': return G;
        case 'H': return H;
        case 'I': return I;
        case 'J': return J;
        case 'K': return K;
        case 'L': return L;
        case 'M': return M;
        case 'N': return N;
        case 'O': return O;
        case 'P': return P;
        case 'Q': return Q;
        case 'R': return R;
        case 'S': return S;
        case 'T': return T;
        case 'U': return U;
        case 'V': return V;
        case 'W': return W;
        case 'X': return X;
        case 'Y': return Y;
        case 'Z': return Z;

        // Simbolos
        case ':': return Colon;
        case '+': return Plus;
        case '-': return Minus;
        case '/': return Slash;
        case '.': return Dot;

        case ' ':
        default:
            return Space;
    }
}

} // namespace Font5x3

#endif 