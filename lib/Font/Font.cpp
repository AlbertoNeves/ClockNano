#include "Font.h"
#include "Font_5x7.h"
#include "Font_5x3.h"

namespace Font
{

    //==========================================================
    // Dimensões
    //==========================================================

    uint8_t width()
    {
        return Font5x7::Width;
    }

    //----------------------------------------------------------

    uint8_t height()
    {
        return Font5x7::Height;
    }

    //----------------------------------------------------------

    uint8_t spacing()
    {
        return Font5x7::Spacing;
    }

    //==========================================================
    // Retorna glyph
    //==========================================================

    const uint8_t *glyph(char c)
    {
        switch (c)
        {
            //==================================================
            // Números
            //==================================================

        case '0':
            return Font5x7::Digit0;
        case '1':
            return Font5x7::Digit1;
        case '2':
            return Font5x7::Digit2;
        case '3':
            return Font5x7::Digit3;
        case '4':
            return Font5x7::Digit4;
        case '5':
            return Font5x7::Digit5;
        case '6':
            return Font5x7::Digit6;
        case '7':
            return Font5x7::Digit7;
        case '8':
            return Font5x7::Digit8;
        case '9':
            return Font5x7::Digit9;

            //==================================================
            // Letras maiúsculas
            //==================================================

        case 'A':
            return Font5x7::A;
        case 'B':
            return Font5x7::B;
        case 'C':
            return Font5x7::C;
        case 'D':
            return Font5x7::D;
        case 'E':
            return Font5x7::E;
        case 'F':
            return Font5x7::F;
        case 'G':
            return Font5x7::G;
        case 'H':
            return Font5x7::H;
        case 'I':
            return Font5x7::I;
        case 'J':
            return Font5x7::J;
        case 'K':
            return Font5x7::K;
        case 'L':
            return Font5x7::L;
        case 'M':
            return Font5x7::M;
        case 'N':
            return Font5x7::N;
        case 'O':
            return Font5x7::O;
        case 'P':
            return Font5x7::P;
        case 'Q':
            return Font5x7::Q;
        case 'R':
            return Font5x7::R;
        case 'S':
            return Font5x7::S;
        case 'T':
            return Font5x7::T;
        case 'U':
            return Font5x7::U;
        case 'V':
            return Font5x7::V;
        case 'W':
            return Font5x7::W;
        case 'X':
            return Font5x7::X;
        case 'Y':
            return Font5x7::Y;
        case 'Z':
            return Font5x7::Z;

            //==================================================
            // Símbolos
            //==================================================

        case ':':
            return Font5x7::Colon;
        case '+':
            return Font5x7::Plus;
        case '-':
            return Font5x7::Minus;
        case '/':
            return Font5x7::Slash;
        case '.':
            return Font5x7::Dot;

            //==================================================
            // Espaço
            //==================================================

        case ' ':
            return Font5x7::Space;

            //==================================================
            // Desconhecido
            //==================================================

        default:
            return Font5x7::Space;
        }
    }
    //==========================================================
    // Fonte pequena 5x3
    //==========================================================

    uint8_t smallWidth()
    {
        return Font5x3::Width;
    }

    //----------------------------------------------------------

    uint8_t smallHeight()
    {
        return Font5x3::Height;
    }

    //----------------------------------------------------------

    uint8_t smallSpacing()
    {
        return Font5x3::Spacing;
    }

    //----------------------------------------------------------

    const uint8_t *smallGlyph(char c)
    {
        return Font5x3::glyph(c);
    }
} // namespace Font
