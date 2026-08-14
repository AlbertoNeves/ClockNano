#pragma once

#include <Arduino.h>

enum class ClockFontStyle : uint8_t
{
    Normal = 0,
    Dual   = 1,
    Seconds = 2
};

namespace Font
{
    // Fonte principal usada pelo menu e pelo texto normal.
    uint8_t width();
    uint8_t height();
    uint8_t spacing();
    const uint8_t *glyph(char c);

    // Fonte pequena 3x5 usada nas telas de edição.
    uint8_t smallWidth();
    uint8_t smallHeight();
    uint8_t smallSpacing();
    const uint8_t *smallGlyph(char c);
}
