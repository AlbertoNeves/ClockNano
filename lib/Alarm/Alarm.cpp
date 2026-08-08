#include "Alarm.h"

#include <Font.h>
#include <avr/pgmspace.h>

namespace
{
    //======================================================
    // Valores da edição
    //======================================================

    uint8_t alarmHour = 0;
    uint8_t alarmMinute = 0;

    //======================================================
    // Estado
    //======================================================

    Alarm::State currentState =
        Alarm::State::Inactive;

    //======================================================
    // Resultado
    //======================================================

    Alarm::Result pendingResult =
        Alarm::Result::None;

    //======================================================
    // Limpa resultado
    //======================================================

    void clearResult()
    {
        pendingResult =
            Alarm::Result::None;
    }

    //======================================================
    // Cria resultado
    //======================================================

    void createResult(
        Alarm::Result result)
    {
        pendingResult = result;
    }

    //======================================================
    // Incrementa hora
    //======================================================

    void increaseHour()
    {
        alarmHour++;

        if (alarmHour > 23)
            alarmHour = 0;
    }

    //======================================================
    // Decrementa hora
    //======================================================

    void decreaseHour()
    {
        if (alarmHour == 0)
            alarmHour = 23;
        else
            alarmHour--;
    }

    //======================================================
    // Incrementa minuto
    //======================================================

    void increaseMinute()
    {
        alarmMinute++;

        if (alarmMinute > 59)
            alarmMinute = 0;
    }

    //======================================================
    // Decrementa minuto
    //======================================================

    void decreaseMinute()
    {
        if (alarmMinute == 0)
            alarmMinute = 59;
        else
            alarmMinute--;
    }

    //======================================================
    // Desenha um caractere da fonte 5x3
    //======================================================

    void drawSmallChar(
        Canvas& canvas,
        char c,
        int16_t x,
        uint8_t y)
    {
        const uint8_t* glyph =
            Font::smallGlyph(c);

        if (glyph == nullptr)
            return;

        const uint8_t width =
            Font::smallWidth();

        const uint8_t height =
            Font::smallHeight();

        for (uint8_t col = 0;
             col < width;
             col++)
        {
            uint8_t data =
                pgm_read_byte(glyph + col);

            for (uint8_t row = 0;
                 row < height;
                 row++)
            {
                if (data & (1 << row))
                {
                    int16_t px =
                        x + col;

                    int16_t py =
                        y + row;

                    if (px >= 0 &&
                        px < canvas.width() &&
                        py >= 0 &&
                        py < canvas.height())
                    {
                        canvas.setPixel(
                            px,
                            py,
                            true);
                    }
                }
            }
        }
    }

    //======================================================
    // Desenha dois dígitos
    //======================================================

    void drawTwoDigits(
        Canvas& canvas,
        uint8_t value,
        int16_t x,
        uint8_t y)
    {
        char tens =
            '0' + (value / 10);

        char units =
            '0' + (value % 10);

        const uint8_t step =
            Font::smallWidth() +
            Font::smallSpacing();

        drawSmallChar(
            canvas,
            tens,
            x,
            y);

        drawSmallChar(
            canvas,
            units,
            x + step,
            y);
    }

    //======================================================
    // Desenha dois pontos do separador HH:MM
    //======================================================

    void drawColon(
        Canvas& canvas,
        int16_t x,
        uint8_t y)
    {
        if (x < 0 || x >= canvas.width())
            return;

        if (y < canvas.height())
        {
            canvas.setPixel(
                x,
                y,
                true);
        }

        if ((y + 2) < canvas.height())
        {
            canvas.setPixel(
                x,
                y + 2,
                true);
        }
    }

    //======================================================
    // Desenha indicador do campo selecionado
    //
    // Hora:
    //     ----
    //
    // Minuto:
    //          ----
    //======================================================

    void drawSelection(
        Canvas& canvas)
    {
        const uint8_t charWidth =
            Font::smallWidth();

        const uint8_t spacing =
            Font::smallSpacing();

        const uint8_t step =
            charWidth + spacing;

        // Largura:
        //
        // HH : MM
        //
        const uint8_t totalWidth =
            (step * 4) + 2;

        const int16_t startX =
            (canvas.width() - totalWidth) / 2;

        const uint8_t y =
            (canvas.height() - Font::smallHeight()) / 2;

        const uint8_t lineY =
            y + Font::smallHeight();

        int16_t x;

        if (currentState ==
            Alarm::State::EditingHour)
        {
            x = startX;
        }
        else
        {
            x = startX +
                (step * 2) +
                2;
        }

        // Pequena linha de seleção
        for (uint8_t i = 0;
             i < (step * 2) - 1;
             i++)
        {
            if ((x + i) < canvas.width())
            {
                canvas.setPixel(
                    x + i,
                    lineY,
                    true);
            }
        }
    }
}

//==========================================================
// API pública
//==========================================================

namespace Alarm
{
    //======================================================
    // Inicialização
    //======================================================

    void begin()
    {
        alarmHour = 0;
        alarmMinute = 0;

        currentState =
            State::Inactive;

        clearResult();
    }

    //======================================================
    // Inicia edição
    //======================================================

    void start()
    {
        alarmHour = 0;
        alarmMinute = 0;

        currentState =
            State::EditingHour;

        clearResult();
    }

    //======================================================
    // Atualização
    //======================================================

    void update(
        const ButtonEvent& event)
    {
        if (currentState ==
            State::Inactive)
        {
            return;
        }

        //==================================================
        // HORA
        //==================================================

        if (currentState ==
            State::EditingHour)
        {
            //------------------------------------------------
            // +
            //------------------------------------------------

            if (event.button ==
                ButtonId::Plus)
            {
                if (event.type ==
                        ButtonEventType::Click ||
                    event.type ==
                        ButtonEventType::Repeat)
                {
                    increaseHour();

                    createResult(
                        Result::Changed);
                }

                return;
            }

            //------------------------------------------------
            // -
            //------------------------------------------------

            if (event.button ==
                ButtonId::Minus)
            {
                if (event.type ==
                        ButtonEventType::Click ||
                    event.type ==
                        ButtonEventType::Repeat)
                {
                    decreaseHour();

                    createResult(
                        Result::Changed);
                }

                return;
            }

            //------------------------------------------------
            // OK
            //------------------------------------------------

            if (event.button ==
                ButtonId::Ok)
            {
                if (event.type ==
                    ButtonEventType::Click)
                {
                    currentState =
                        State::EditingMinute;

                    createResult(
                        Result::HourConfirmed);
                }

                return;
            }
        }

        //==================================================
        // MINUTO
        //==================================================

        if (currentState ==
            State::EditingMinute)
        {
            //------------------------------------------------
            // +
            //------------------------------------------------

            if (event.button ==
                ButtonId::Plus)
            {
                if (event.type ==
                        ButtonEventType::Click ||
                    event.type ==
                        ButtonEventType::Repeat)
                {
                    increaseMinute();

                    createResult(
                        Result::Changed);
                }

                return;
            }

            //------------------------------------------------
            // -
            //------------------------------------------------

            if (event.button ==
                ButtonId::Minus)
            {
                if (event.type ==
                        ButtonEventType::Click ||
                    event.type ==
                        ButtonEventType::Repeat)
                {
                    decreaseMinute();

                    createResult(
                        Result::Changed);
                }

                return;
            }

            //------------------------------------------------
            // OK
            //------------------------------------------------

            if (event.button ==
                ButtonId::Ok)
            {
                if (event.type ==
                    ButtonEventType::Click)
                {
                    currentState =
                        State::Inactive;

                    createResult(
                        Result::Confirmed);
                }

                return;
            }
        }
    }

    //======================================================
    // Renderização
    //======================================================

    void draw(
        Canvas& canvas)
    {
        if (currentState ==
            State::Inactive)
        {
            return;
        }

        canvas.clear();

        const uint8_t charWidth =
            Font::smallWidth();

        const uint8_t spacing =
            Font::smallSpacing();

        const uint8_t step =
            charWidth + spacing;

        //--------------------------------------------------
        // HH : MM
        //--------------------------------------------------

        const uint8_t totalWidth =
            (step * 4) + 2;

        const int16_t startX =
            (canvas.width() - totalWidth) / 2;

        const uint8_t y =
            (canvas.height() -
             Font::smallHeight()) / 2;

        //--------------------------------------------------
        // HORA
        //--------------------------------------------------

        drawTwoDigits(
            canvas,
            alarmHour,
            startX,
            y);

        //--------------------------------------------------
        // :
        //--------------------------------------------------

        const int16_t colonX =
            startX +
            (step * 2);

        drawColon(
            canvas,
            colonX,
            y + 1);

        //--------------------------------------------------
        // MINUTO
        //--------------------------------------------------

        const int16_t minuteX =
            colonX + 2;

        drawTwoDigits(
            canvas,
            alarmMinute,
            minuteX,
            y);

        //--------------------------------------------------
        // Indicador do campo
        //--------------------------------------------------

        drawSelection(canvas);
    }

    //======================================================
    // Estado
    //======================================================

    State state()
    {
        return currentState;
    }

    //======================================================
    // Resultado
    //======================================================

    bool readResult(
        Result& result)
    {
        if (pendingResult ==
            Result::None)
        {
            return false;
        }

        result =
            pendingResult;

        clearResult();

        return true;
    }

    //======================================================
    // Hora
    //======================================================

    uint8_t hour()
    {
        return alarmHour;
    }

    //======================================================
    // Minuto
    //======================================================

    uint8_t minute()
    {
        return alarmMinute;
    }

    //======================================================
    // Cancelar
    //======================================================

    void cancel()
    {
        currentState =
            State::Inactive;

        clearResult();
    }
}