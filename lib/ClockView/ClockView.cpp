#include "ClockView.h"

#include <Canvas.h>
#include <Graphics.h>
#include <Font.h>
#include <Font_5x7.h>
#include <Font_Dual.h>
#include <Font_5x3.h>
#include <Font_Round.h>
#include <avr/pgmspace.h>

namespace
{
    ClockFontStyle currentFont = ClockFontStyle::Normal;

    //======================================================
    // Animação dos segundos — V6 "Seconds Odometer"
    //======================================================

    uint8_t lastSecond = 255;
    uint8_t oldSecond = 0;
    uint8_t newSecond = 0;
    uint32_t animationStart = 0;
    bool scrolling = false;

    // Layout FIXO para a tela principal.
    // Não usamos centralização automática no relógio: cada elemento tem
    // posição física definida para aproveitar corretamente os 32x8 pixels.
    //
    // NORMAL (5x7): cada elemento ocupa 5 colunas e há 1 coluna apagada
    // entre hora, dois-pontos e minutos. Layout fixo: 1 + 5 + 1 + 5 + 2 + 5 + 2 + 5 + 1 + 5 = 32 colunas.
    constexpr uint8_t NormalHourDigit1X = 3;
    constexpr uint8_t NormalHourDigit2X = 9;
    constexpr uint8_t NormalColonX = 14;
    constexpr uint8_t NormalMinuteDigit1X = 18;
    constexpr uint8_t NormalMinuteDigit2X = 24;

    // SEGUNDOS: HH:MM usa a dig5x8rn (5x8). Mantemos 1 px entre os
    // elementos de HH:MM; o bloco dos segundos começa imediatamente após
    // o último dígito para caber nos 32 pixels.
    // 5 + 1 + 5 + 1 + 1 + 1 + 5 + 1 + 5 = 25 pixels.
    // Os segundos 3x5 ocupam as 7 colunas restantes (25..31).
    constexpr uint8_t SecondsClockHourDigit1X = 0;
    constexpr uint8_t SecondsClockHourDigit2X = 6;
    constexpr uint8_t SecondsClockColonX = 12;
    constexpr uint8_t SecondsClockMinuteDigit1X = 14;
    constexpr uint8_t SecondsClockMinuteDigit2X = 20;
    constexpr uint8_t SecondsX = 25;
    constexpr uint8_t SecondsDigitWidth = Font5x3::Width;
    constexpr uint8_t SecondsDigitGap = 1;

    // Fonte 3x5: permanece aprovada para os segundos.
    // Centralizada verticalmente no display 8x8.
    constexpr int16_t SecondsY = 1;

    // Duração total da transição de um segundo para o próximo.
    constexpr uint16_t ScrollDurationMs = 200;

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

    void drawNormalClock(Canvas &canvas, uint8_t hour, uint8_t minute, bool showColon)
    {
        drawGlyph(canvas, NormalHourDigit1X, 0,
                  Font::glyph('0' + hour / 10), 5, 7);

        drawGlyph(canvas, NormalHourDigit2X, 0,
                  Font::glyph('0' + hour % 10), 5, 7);

        if (showColon)
        {
            drawGlyph(canvas, NormalColonX, 0,
                      Font5x7::Colon, Font5x7::Width, Font5x7::Height);
        }

        drawGlyph(canvas, NormalMinuteDigit1X, 0,
                  Font::glyph('0' + minute / 10), 5, 7);

        drawGlyph(canvas, NormalMinuteDigit2X, 0,
                  Font::glyph('0' + minute % 10), 5, 7);
    }

    void drawSecondsClockTime(Canvas &canvas, uint8_t hour, uint8_t minute, bool showColon)
    {
        // HH:MM em dig5x8rn, sem centralização automática.
        drawGlyph(canvas, SecondsClockHourDigit1X, 0,
                  FontRound::glyph('0' + hour / 10),
                  FontRound::Width, FontRound::Height);

        drawGlyph(canvas, SecondsClockHourDigit2X, 0,
                  FontRound::glyph('0' + hour % 10),
                  FontRound::Width, FontRound::Height);

        if (showColon)
        {
            // Dois pontos compactos de 1 coluna, mantendo 1 px livre
            // antes e depois, como nos demais elementos.
            static const uint8_t compactColon[1] PROGMEM = {0x66};
            drawGlyph(canvas, SecondsClockColonX, 0, compactColon, 1, 8);
        }

        drawGlyph(canvas, SecondsClockMinuteDigit1X, 0,
                  FontRound::glyph('0' + minute / 10),
                  FontRound::Width, FontRound::Height);

        drawGlyph(canvas, SecondsClockMinuteDigit2X, 0,
                  FontRound::glyph('0' + minute % 10),
                  FontRound::Width, FontRound::Height);
    }

    void drawDualClock(Canvas &canvas, uint8_t hour, uint8_t minute, bool showColon)
    {
        // dig6x8: 6x8 por dígito.
        // Layout com 1 coluna livre entre cada elemento:
        // HHHHHH _ HHHHHH _ :: _ MMMMMM _ MMMMMM
        // 24 colunas dos 4 dígitos + 2 do ':' + 4 espaços = 30.
        // Ficam 1 coluna livre em cada extremidade do display 32x8.
        constexpr uint8_t hour1X = 1;
        constexpr uint8_t hour2X = 8;
        constexpr uint8_t colonX = 15;
        constexpr uint8_t minute1X = 18;
        constexpr uint8_t minute2X = 25;

        drawGlyph(canvas, hour1X, 0,
                  FontDual::glyph('0' + hour / 10),
                  FontDual::Width, FontDual::Height);

        drawGlyph(canvas, hour2X, 0,
                  FontDual::glyph('0' + hour % 10),
                  FontDual::Width, FontDual::Height);

        if (showColon)
        {
            drawGlyph(canvas, colonX, 0,
                      FontDual::Colon, 2, FontDual::Height);
        }

        drawGlyph(canvas, minute1X, 0,
                  FontDual::glyph('0' + minute / 10),
                  FontDual::Width, FontDual::Height);

        drawGlyph(canvas, minute2X, 0,
                  FontDual::glyph('0' + minute % 10),
                  FontDual::Width, FontDual::Height);
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
            animationStart = millis();
            scrolling = true;
            lastSecond = second;
        }

        if (!scrolling)
            return;

        const uint32_t elapsed = millis() - animationStart;
        if (elapsed >= ScrollDurationMs)
        {
            scrolling = false;
        }
    }

    // Curva smoothstep: começa devagar, acelera no meio e desacelera
    // suavemente antes de terminar. O resultado é mais natural que uma
    // rolagem linear.
    uint8_t easedDisplacement()
    {
        const uint32_t elapsed = millis() - animationStart;

        if (elapsed >= ScrollDurationMs)
            return Font5x3::Height + 1;

        const uint32_t t = (elapsed * 255UL) / ScrollDurationMs;

        // smoothstep aproximado em aritmética inteira:
        // f(t) = t²(3 - 2t), com t normalizado em 0..255.
        const uint32_t t2 = t * t;
        const uint32_t smooth =
            (t2 * (765UL - 2UL * t)) / (255UL * 255UL);

        return static_cast<uint8_t>(
            (smooth * (Font5x3::Height + 1) + 127UL) / 255UL);
    }

    void drawAnimatedDigit(
        Canvas &canvas,
        uint8_t oldDigit,
        uint8_t newDigit,
        uint8_t x,
        uint8_t displacement)
    {
        // Se o dígito não mudou, permanece completamente parado.
        if (oldDigit == newDigit || !scrolling)
        {
            drawGlyph(canvas, x, SecondsY,
                      Font5x3::glyph('0' + newDigit),
                      Font5x3::Width, Font5x3::Height);
            return;
        }

        // O dígito antigo sobe e desaparece pelo topo.
        const int16_t oldY = SecondsY - displacement;

        // O novo dígito entra por baixo e termina exatamente na posição
        // normal do indicador de segundos.
        const int16_t newY = SecondsY + Font5x3::Height + 1 - displacement;

        drawGlyph(canvas, x, oldY,
                  Font5x3::glyph('0' + oldDigit),
                  Font5x3::Width, Font5x3::Height);

        drawGlyph(canvas, x, newY,
                  Font5x3::glyph('0' + newDigit),
                  Font5x3::Width, Font5x3::Height);
    }

    void drawScrollingSeconds(Canvas &canvas, uint8_t second)
    {
        updateScroll(second);

        const uint8_t oldTens = oldSecond / 10;
        const uint8_t oldUnits = oldSecond % 10;
        const uint8_t newTens = newSecond / 10;
        const uint8_t newUnits = newSecond % 10;

        if (!scrolling)
        {
            drawGlyph(canvas, SecondsX, SecondsY,
                      Font5x3::glyph('0' + newTens),
                      Font5x3::Width, Font5x3::Height);

            drawGlyph(canvas,
                      SecondsX + SecondsDigitWidth + SecondsDigitGap,
                      SecondsY,
                      Font5x3::glyph('0' + newUnits),
                      Font5x3::Width, Font5x3::Height);
            return;
        }

        const uint8_t displacement = easedDisplacement();

        // Comportamento de odômetro:
        // 37 -> 38 : somente a unidade rola.
        // 39 -> 40 : dezena e unidade rolam juntas.
        // 59 -> 00 : os dois dígitos rolam simultaneamente.
        drawAnimatedDigit(
            canvas,
            oldTens,
            newTens,
            SecondsX,
            displacement);

        drawAnimatedDigit(
            canvas,
            oldUnits,
            newUnits,
            SecondsX + SecondsDigitWidth + SecondsDigitGap,
            displacement);

        // A separação entre o dígito que sai e o que entra é uma
        // linha de pixels APAGADOS. Como o Canvas é limpo a cada quadro,
        // basta manter uma distância de 1 pixel entre os dois glyphs.
        // Não desenhamos nenhum pixel nessa linha.
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
        // A fonte SEGUNDOS usa dig5x8rn para HH:MM e mantém as 7 colunas
        // reservadas para o indicador SS em fonte 3x5.
        if (currentFont == ClockFontStyle::Seconds)
        {
            drawSecondsClockTime(canvas, hour, minute, showColon);
            drawScrollingSeconds(canvas, second);
            return;
        }

        if (currentFont == ClockFontStyle::Dual)
        {
            drawDualClock(canvas, hour, minute, showColon);
            return;
        }

        // O relógio usa posições fixas; a centralização automática fica
        // reservada para textos e telas que realmente precisam dela.
        drawNormalClock(canvas, hour, minute, showColon);
    }
}
