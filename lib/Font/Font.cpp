#include "Font.h"
#include "Font_5x7.h"

namespace Font
{

uint8_t width()
{
    return 5;
}

uint8_t height()
{
    return 7;
}

uint8_t spacing()
{
    return 1;
}

const uint8_t* glyph(char c)
{
    switch (c)
    {
        case '0': return Font5x7::Digit0;
        case '1': return Font5x7::Digit1;
        case '2': return Font5x7::Digit2;
        case '3': return Font5x7::Digit3;
        case '4': return Font5x7::Digit4;
        case '5': return Font5x7::Digit5;
        case '6': return Font5x7::Digit6;
        case '7': return Font5x7::Digit7;
        case '8': return Font5x7::Digit8;
        case '9': return Font5x7::Digit9;

        case ':': return Font5x7::Colon;

        default:
            return Font5x7::Space;
    }
}

}