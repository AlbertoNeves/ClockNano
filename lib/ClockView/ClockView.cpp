#include "ClockView.h"

#include <Canvas.h>
#include <Graphics.h>
#include <Font.h>
#include <Font_Dual.h>
#include <Font_5x3.h>
#include <avr/pgmspace.h>

namespace
{
    ClockFontStyle currentFont = ClockFontStyle::Normal;

    // Estado da animação vertical dos segundos.
    uint8_t lastSecond = 255;
    uint8_t oldSecond = 0;
    uint8_t newSecond = 0;
    uint8_t scrollOffset = 0;
    uint32_t lastScrollStep = 0;
    bool scrolling = false;

    // Layout compacto do modo SEGUNDOS para os 32 pixels do display.
    // HH:MM = 25 colunas; SS = 7 colunas. Total = 32.
    constexpr uint8_t HourDigit1X = 0;
    constexpr uint8_t HourDigit2X = 6;
    constexpr uint8_t ColonX = 11;
    constexpr uint8_t MinuteDigit1X = 14;
    constexpr uint8_t MinuteDigit2X = 20;
    constexpr uint8_t SecondsX = 25;
    constexpr uint8_t SecondsDigitWidth = Font5x3::Width;
    constexpr uint8_t SecondsDigitGap = 1;
    constexpr uint8_t DigitH = Font5x3::Height;
    constexpr uint16_t ScrollStepMs = 25;

    void drawGlyph(
        Canvas &canvas,
        int16_t x,
        int16_t y,
        const uint8_t *glyph,
        uint8_t width,
        uint8_t height)
    {
        if (glyph == nullptr)
            return;

        for (uint8_t col = 0; col < width; ++col)
        {
            int16_t sx = x + col;
            if (sx < 0 || sx >= canvas.width())
                continue;

            uint8_t data = pgm_read_byte(glyph + col);

            for (uint8_t row = 0; row < height; ++row)
            {
                int16_t sy = y + row;
                if (sy < 0 || sy >= canvas.height())
                    continue;

                if (data & (1 << row))
                    canvas.setPixel(sx, sy, true);
            }
        }
    }

    void drawDualClock(Canvas &canvas, uint8_t hour, uint8_t minute, bool showColon)
    {
        char txt[6];
        snprintf(txt, sizeof(txt), "%02u:%02u", hour, minute);
        if (!showColon)
            txt[2] = ' ';

        constexpr uint8_t charW = FontDual::Width;
        constexpr uint8_t spacing = FontDual::Spacing;
        constexpr uint8_t totalW = 5 * charW + 4 * spacing;
        const uint8_t x0 = (canvas.width() - totalW) / 2;

        for (uint8_t i = 0; i < 5; ++i)
            drawGlyph(canvas, x0 + i * (charW + spacing), 0,
                      FontDual::glyph(txt[i]), charW, FontDual::Height);
    }

    void drawSecondsBlock(
        Canvas &canvas,
        uint8_t seconds,
        int16_t y,
        bool clipped)
    {
        (void)clipped;
        char tens = '0' + (seconds / 10);
        char units = '0' + (seconds % 10);

        // Dois dígitos 5x3 lado a lado: TT UU.
        drawGlyph(canvas, SecondsX, y,
                  Font5x3::glyph(tens),
                  Font5x3::Width, Font5x3::Height);
        drawGlyph(canvas, SecondsX + SecondsDigitWidth + SecondsDigitGap, y,
                  Font5x3::glyph(units),
                  Font5x3::Width, Font5x3::Height);
    }

    void resetScroll(uint8_t second)
    {
        oldSecond = (second == 0) ? 59 : second - 1;
        newSecond = second;
        scrollOffset = 0;
        scrolling = true;
        lastScrollStep = millis();
    }

    void updateScroll(uint8_t second)
    {
        if (lastSecond == 255)
        {
            lastSecond = second;
            oldSecond = second;
            newSecond = second;
            scrolling = false;
            return;
        }

        if (second != lastSecond)
        {
            oldSecond = lastSecond;
            newSecond = second;
            scrollOffset = 0;
            scrolling = true;
            lastScrollStep = millis();
            lastSecond = second;
        }

        if (!scrolling)
            return;

        const uint32_t now = millis();
        if ((now - lastScrollStep) < ScrollStepMs)
            return;

        lastScrollStep = now;
        ++scrollOffset;

        if (scrollOffset >= 8)
        {
            scrollOffset = 0;
            scrolling = false;
        }
    }

    void drawScrollingSeconds(Canvas &canvas, uint8_t second)
    {
        updateScroll(second);

        // Os dois dígitos 5x3 ocupam 7 colunas e são centralizados
        // verticalmente na área 8x8.
        if (!scrolling)
        {
            drawSecondsBlock(canvas, newSecond, 2, false);
            return;
        }

        // Rolagem vertical: o valor antigo sai por cima e o novo entra
        // por baixo. O grupo SS tem 5 pixels de altura.
        const int16_t oldY = -static_cast<int16_t>(scrollOffset);
        const int16_t newY = 8 - static_cast<int16_t>(scrollOffset);

        drawSecondsBlock(canvas, oldSecond, oldY, true);
        drawSecondsBlock(canvas, newSecond, newY, true);
    }

}

namespace ClockView
{
    void setFont(ClockFontStyle style)
    {
        if (static_cast<uint8_t>(style) > static_cast<uint8_t>(ClockFontStyle::Seconds))
            style = ClockFontStyle::Normal;

        currentFont = style;

        if (currentFont != ClockFontStyle::Seconds)
        {
            scrolling = false;
            lastSecond = 255;
        }
    }

    ClockFontStyle font()
    {
        return currentFont;
    }

    void draw(
        Canvas& canvas,
        uint8_t hour,
        uint8_t minute,
        uint8_t second,
        bool showColon)
    {
        // A fonte SEGUNDOS mantém o horário em Font5x7 e usa as 7 colunas
        // reservadas para o indicador SS em fonte 5x3.
        if (currentFont == ClockFontStyle::Seconds)
        {
            // Layout compacto específico para 32x8.
            //
            // HH = 5 + 1 + 5
            // :  = 3 colunas, sem espaçamento externo
            // MM = 5 + 1 + 5
            // SS = 3 + 1 + 3
            //
            // Total = 25 + 7 = 32 pixels.
            const uint8_t h10 = hour / 10;
            const uint8_t h01 = hour % 10;
            const uint8_t m10 = minute / 10;
            const uint8_t m01 = minute % 10;

            drawGlyph(canvas, HourDigit1X, 0,
                      Font::glyph('0' + h10), 5, 7);
            drawGlyph(canvas, HourDigit2X, 0,
                      Font::glyph('0' + h01), 5, 7);

            // ':' compacto: uma única coluna de largura, dentro de uma
            // área lógica de 3 colunas. Isso elimina o espaçamento externo
            // e mantém os dois pontos visualmente separados dos dígitos.
            if (showColon)
            {
                static const uint8_t compactColon[1] PROGMEM = {0x36};
                drawGlyph(canvas, ColonX + 1, 0, compactColon, 1, 7);
            }

            drawGlyph(canvas, MinuteDigit1X, 0,
                      Font::glyph('0' + m10), 5, 7);
            drawGlyph(canvas, MinuteDigit2X, 0,
                      Font::glyph('0' + m01), 5, 7);

            drawScrollingSeconds(canvas, second);
            return;
        }

        if (currentFont == ClockFontStyle::Dual)
        {
            drawDualClock(canvas, hour, minute, showColon);
            return;
        }

        char txt[6];
        snprintf(txt, sizeof(txt), "%02u:%02u", hour, minute);
        if (!showColon)
            txt[2] = ' ';

        Graphics::drawStringCentered(canvas, 0, txt);
    }
}
