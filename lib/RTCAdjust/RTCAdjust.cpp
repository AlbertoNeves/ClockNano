#include "RTCAdjust.h"
#include <Font.h>
#include <avr/pgmspace.h>

#include <Arduino.h>
#include <RTC.h>

namespace
{
    RTCAdjust::Mode editMode =
        RTCAdjust::Mode::Date;

    RTCAdjust::State currentState =
        RTCAdjust::State::Inactive;

    RTCAdjust::Result pendingResult =
        RTCAdjust::Result::None;

    RtcDateTime editingDateTime;

    //------------------------------------------------------
    // Limpa resultado pendente
    //------------------------------------------------------

    void clearResult()
    {
        pendingResult =
            RTCAdjust::Result::None;
    }

    //------------------------------------------------------
    // Cria resultado
    //------------------------------------------------------

    void createResult(
        RTCAdjust::Result result)
    {
        pendingResult = result;
    }

    //------------------------------------------------------
    // Ano bissexto
    //------------------------------------------------------

    bool isLeapYear(
        uint16_t year)
    {
        if ((year % 400) == 0)
            return true;

        if ((year % 100) == 0)
            return false;

        return ((year % 4) == 0);
    }

    //------------------------------------------------------
    // Dias do mês
    //------------------------------------------------------

    uint8_t daysInMonth(
        uint16_t year,
        uint8_t month)
    {
        switch (month)
        {
        case 2:
            return isLeapYear(year)
                       ? 29
                       : 28;

        case 4:
        case 6:
        case 9:
        case 11:
            return 30;

        default:
            return 31;
        }
    }

    //------------------------------------------------------
    // Limita o dia quando mês/ano é alterado
    //------------------------------------------------------

    void validateDay()
    {
        uint8_t maxDay =
            daysInMonth(
                editingDateTime.year,
                editingDateTime.month);

        if (editingDateTime.day > maxDay)
        {
            editingDateTime.day =
                maxDay;
        }
    }

    //------------------------------------------------------
    // Incrementa ano
    //------------------------------------------------------

    void increaseYear()
    {
        if (editingDateTime.year >= 2099)
        {
            editingDateTime.year = 2020;
        }
        else
        {
            editingDateTime.year++;
        }

        Serial.print("RTC YEAR = ");
        Serial.println(editingDateTime.year);

        validateDay();
    }

    //------------------------------------------------------
    // Decrementa ano
    //------------------------------------------------------

    void decreaseYear()
    {
        if (editingDateTime.year <= 2020)
        {
            editingDateTime.year = 2099;
        }
        else
        {
            editingDateTime.year--;
        }

        Serial.print("RTC YEAR = ");
        Serial.println(editingDateTime.year);

        validateDay();
    }

    //------------------------------------------------------
    // Incrementa mês
    //------------------------------------------------------

    void increaseMonth()
    {
        if (editingDateTime.month >= 12)
        {
            editingDateTime.month = 1;
        }
        else
        {
            editingDateTime.month++;
        }

        validateDay();
    }

    //------------------------------------------------------
    // Decrementa mês
    //------------------------------------------------------

    void decreaseMonth()
    {
        if (editingDateTime.month <= 1)
        {
            editingDateTime.month = 12;
        }
        else
        {
            editingDateTime.month--;
        }

        validateDay();
    }

    //------------------------------------------------------
    // Incrementa dia
    //------------------------------------------------------

    void increaseDay()
    {
        uint8_t maxDay =
            daysInMonth(
                editingDateTime.year,
                editingDateTime.month);

        if (editingDateTime.day >= maxDay)
        {
            editingDateTime.day = 1;
        }
        else
        {
            editingDateTime.day++;
        }
    }

    //------------------------------------------------------
    // Decrementa dia
    //------------------------------------------------------

    void decreaseDay()
    {
        if (editingDateTime.day <= 1)
        {
            editingDateTime.day =
                daysInMonth(
                    editingDateTime.year,
                    editingDateTime.month);
        }
        else
        {
            editingDateTime.day--;
        }
    }

    //------------------------------------------------------
    // Incrementa hora
    //------------------------------------------------------

    void increaseHour()
    {
        if (editingDateTime.hour >= 23)
        {
            editingDateTime.hour = 0;
        }
        else
        {
            editingDateTime.hour++;
        }
    }

    //------------------------------------------------------
    // Decrementa hora
    //------------------------------------------------------

    void decreaseHour()
    {
        if (editingDateTime.hour == 0)
        {
            editingDateTime.hour = 23;
        }
        else
        {
            editingDateTime.hour--;
        }
    }

    //------------------------------------------------------
    // Incrementa minuto
    //------------------------------------------------------

    void increaseMinute()
    {
        if (editingDateTime.minute >= 59)
        {
            editingDateTime.minute = 0;
        }
        else
        {
            editingDateTime.minute++;
        }
    }

    //------------------------------------------------------
    // Decrementa minuto
    //------------------------------------------------------

    void decreaseMinute()
    {
        if (editingDateTime.minute == 0)
        {
            editingDateTime.minute = 59;
        }
        else
        {
            editingDateTime.minute--;
        }
    }
    //======================================================
    // Desenha caractere usando fonte pequena
    //======================================================

    void drawSmallChar(
        Canvas &canvas,
        char c,
        int16_t x,
        uint8_t y)
    {
        const uint8_t *glyph =
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
    // Desenha texto usando fonte pequena
    //======================================================

    void drawSmallText(
        Canvas &canvas,
        const char *text,
        uint8_t y)
    {
        const uint8_t width =
            Font::smallWidth();

        const uint8_t spacing =
            Font::smallSpacing();

        const uint8_t step =
            width + spacing;

        uint8_t length = 0;

        while (text[length] != '\0')
            length++;

        if (length == 0)
            return;

        const uint8_t totalWidth =
            (length * step) - spacing;

        int16_t x =
            (canvas.width() - totalWidth) / 2;

        for (uint8_t i = 0;
             i < length;
             i++)
        {
            drawSmallChar(
                canvas,
                text[i],
                x,
                y);

            x += step;
        }
    }

    //======================================================
    // Desenha dois dígitos
    //======================================================

    void drawTwoDigits(
        Canvas &canvas,
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
    // Desenha quatro dígitos
    //======================================================

    void drawFourDigits(
        Canvas &canvas,
        uint16_t value,
        int16_t x,
        uint8_t y)
    {
        const uint8_t step =
            Font::smallWidth() +
            Font::smallSpacing();

        drawSmallChar(
            canvas,
            '0' + ((value / 1000) % 10),
            x,
            y);

        drawSmallChar(
            canvas,
            '0' + ((value / 100) % 10),
            x + step,
            y);

        drawSmallChar(
            canvas,
            '0' + ((value / 10) % 10),
            x + (step * 2),
            y);

        drawSmallChar(
            canvas,
            '0' + (value % 10),
            x + (step * 3),
            y);
    }

    //======================================================
    // Desenha "/"
    //======================================================

    void drawSlash(
        Canvas &canvas,
        int16_t x,
        uint8_t y)
    {
        if (x < 0 ||
            x >= canvas.width())
        {
            return;
        }

        if (y < canvas.height())
        {
            canvas.setPixel(
                x,
                y,
                true);
        }

        if ((y + 1) < canvas.height())
        {
            canvas.setPixel(
                x,
                y + 1,
                true);
        }

        if ((y + 2) < canvas.height())
        {
            canvas.setPixel(
                x,
                y + 2,
                true);
        }

        if ((y + 3) < canvas.height())
        {
            canvas.setPixel(
                x,
                y + 3,
                true);
        }

        if ((y + 4) < canvas.height())
        {
            canvas.setPixel(
                x,
                y + 4,
                true);
        }
    }

    //======================================================
    // Desenha ":"
    //======================================================

    void drawColon(
        Canvas &canvas,
        int16_t x,
        uint8_t y)
    {
        if (x < 0 ||
            x >= canvas.width())
        {
            return;
        }

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
    // Desenha linha sob o campo selecionado
    //======================================================

    void drawUnderline(
        Canvas &canvas,
        int16_t x,
        uint8_t y,
        uint8_t width)
    {
        for (uint8_t i = 0;
             i < width;
             i++)
        {
            if ((x + i) >= 0 &&
                (x + i) < canvas.width() &&
                y < canvas.height())
            {
                canvas.setPixel(
                    x + i,
                    y,
                    true);
            }
        }
    }
}

namespace RTCAdjust
{
    //------------------------------------------------------
    // Inicialização
    //------------------------------------------------------

    void begin()
    {
        currentState =
            State::Inactive;

        clearResult();

        editingDateTime =
            RTC::now();
    }

    //------------------------------------------------------
    // Inicia ajuste
    //------------------------------------------------------

    void start(Mode mode)
    {
        editMode = mode;

        editingDateTime =
            RTC::now();

        clearResult();

        if (editMode ==
            Mode::Date)
        {
            currentState =
                State::EditingDay;
        }
        else
        {
            currentState =
                State::EditingHour;
        }
    }

    //------------------------------------------------------
    // Atualização
    //------------------------------------------------------

    void update(
        const ButtonEvent &event)
    {
        Serial.print("RTC BUTTON: ");
        Serial.print(
            static_cast<uint8_t>(event.button));

        Serial.print(" TYPE: ");
        Serial.println(
            static_cast<uint8_t>(event.type));

        if (currentState ==
            State::Inactive)
        {
            return;
        }

        //--------------------------------------------------
        // HOME = cancela
        //--------------------------------------------------

        if (event.button ==
            ButtonId::Home)
        {
            if (event.type ==
                ButtonEventType::Click)
            {
                currentState =
                    State::Inactive;

                createResult(
                    Result::Cancelled);
            }

            return;
        }

        //--------------------------------------------------
        // +
        //--------------------------------------------------

        if (event.button ==
            ButtonId::Plus)
        {
            Serial.print("RTC PLUS - STATE: ");
            Serial.println(
                static_cast<uint8_t>(currentState));

            if (event.type ==
                    ButtonEventType::Click ||
                event.type ==
                    ButtonEventType::Repeat)
            {
                switch (currentState)
                {
                case State::EditingYear:
                    increaseYear();
                    break;

                case State::EditingMonth:
                    increaseMonth();
                    break;

                case State::EditingDay:
                    increaseDay();
                    break;

                case State::EditingHour:
                    increaseHour();
                    break;

                case State::EditingMinute:
                    increaseMinute();
                    break;

                default:
                    break;
                }

                createResult(
                    Result::Changed);
            }

            return;
        }

        //--------------------------------------------------
        // -
        //--------------------------------------------------

        if (event.button ==
            ButtonId::Minus)
        {
            if (event.type ==
                    ButtonEventType::Click ||
                event.type ==
                    ButtonEventType::Repeat)
            {
                switch (currentState)
                {
                case State::EditingYear:
                    decreaseYear();
                    break;

                case State::EditingMonth:
                    decreaseMonth();
                    break;

                case State::EditingDay:
                    decreaseDay();
                    break;

                case State::EditingHour:
                    decreaseHour();
                    break;

                case State::EditingMinute:
                    decreaseMinute();
                    break;

                default:
                    break;
                }

                createResult(
                    Result::Changed);
            }

            return;
        }

        //--------------------------------------------------
        // OK
        //--------------------------------------------------

        //------------------------------------------------------
        // OK
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Ok)
        {
            if (event.type ==
                ButtonEventType::Click)
            {
                //--------------------------------------------------
                // MODO DATA
                //--------------------------------------------------

                if (editMode ==
                    RTCAdjust::Mode::Date)
                {
                    switch (currentState)
                    {
                    case State::EditingDay:

                        currentState =
                            State::EditingMonth;

                        createResult(
                            Result::FieldConfirmed);

                        return;

                    case State::EditingMonth:

                        currentState =
                            State::EditingYear;

                        createResult(
                            Result::FieldConfirmed);

                        return;

                    case State::EditingYear:

                        currentState =
                            State::Confirm;

                        createResult(
                            Result::FieldConfirmed);

                        return;

                    case State::Confirm:

                        RTC::adjust(
                            editingDateTime);

                        currentState =
                            State::Inactive;

                        createResult(
                            Result::Confirmed);

                        return;

                    default:
                        return;
                    }
                }

                //--------------------------------------------------
                // MODO HORA
                //--------------------------------------------------

                if (editMode ==
                    RTCAdjust::Mode::Time)
                {
                    switch (currentState)
                    {
                    case State::EditingHour:

                        currentState =
                            State::EditingMinute;

                        createResult(
                            Result::FieldConfirmed);

                        return;

                    case State::EditingMinute:

                        currentState =
                            State::Confirm;

                        createResult(
                            Result::FieldConfirmed);

                        return;

                    case State::Confirm:

                        RTC::adjust(
                            editingDateTime);

                        currentState =
                            State::Inactive;

                        createResult(
                            Result::Confirmed);

                        return;

                    default:
                        return;
                    }
                }
            }

            return;
        }
    }

    //------------------------------------------------------
    // Desenho
    //------------------------------------------------------

    //======================================================
    // Renderização do ajuste RTC
    //======================================================
   void draw(
    Canvas &canvas)
{
    if (currentState ==
        State::Inactive)
    {
        return;
    }

    canvas.clear();


    //------------------------------------------------------
    // CONFIGURAÇÃO DA FONTE
    //------------------------------------------------------

    const uint8_t W =
        Font::smallWidth();

    const uint8_t S =
        Font::smallSpacing();

    const uint8_t STEP =
        W + S;

    const uint8_t H =
        Font::smallHeight();


    //------------------------------------------------------
    // POSIÇÃO VERTICAL
    //
    // Fonte 5x3:
    //
    // linha 0
    // linha 1
    // linha 2
    //
    // cursor na linha 4
    //------------------------------------------------------

    const uint8_t y = 1;

    const uint8_t cursorY =
        y + H + 1;


    //------------------------------------------------------
    // DATA
    //
    // DD  MM  AA
    //------------------------------------------------------

    if (editMode ==
        RTCAdjust::Mode::Date)
    {
        /*
         *
         *       11 08 26
         *
         */

        const uint8_t gap = 2;

        const uint8_t fieldWidth =
            STEP * 2;

        const uint8_t totalWidth =
            fieldWidth +
            gap +
            fieldWidth +
            gap +
            fieldWidth;

        const int16_t startX =
            (canvas.width() -
             totalWidth) / 2;


        //--------------------------------------------------
        // DIA
        //--------------------------------------------------

        drawTwoDigits(
            canvas,
            editingDateTime.day,
            startX,
            y);


        //--------------------------------------------------
        // MÊS
        //--------------------------------------------------

        const int16_t monthX =
            startX +
            fieldWidth +
            gap;

        drawTwoDigits(
            canvas,
            editingDateTime.month,
            monthX,
            y);


        //--------------------------------------------------
        // ANO - somente 2 dígitos
        //--------------------------------------------------

        const int16_t yearX =
            monthX +
            fieldWidth +
            gap;

        drawTwoDigits(
            canvas,
            editingDateTime.year % 100,
            yearX,
            y);


        //--------------------------------------------------
        // CURSOR
        //--------------------------------------------------

        int16_t cursorX =
            startX;


        if (currentState ==
            State::EditingMonth)
        {
            cursorX =
                monthX;
        }
        else if (currentState ==
                 State::EditingYear)
        {
            cursorX =
                yearX;
        }


        if (currentState ==
                State::EditingDay ||
            currentState ==
                State::EditingMonth ||
            currentState ==
                State::EditingYear)
        {
            for (uint8_t i = 0;
                 i < fieldWidth - S;
                 i++)
            {
                if ((cursorX + i) >= 0 &&
                    (cursorX + i) <
                        canvas.width())
                {
                    canvas.setPixel(
                        cursorX + i,
                        cursorY,
                        true);
                }
            }
        }

        return;
    }


    //------------------------------------------------------
    // HORA
    //
    // HH  MM
    //------------------------------------------------------

    if (editMode ==
        RTCAdjust::Mode::Time)
    {
        /*
         *
         *         19 42
         *
         */

        const uint8_t gap = 2;

        const uint8_t fieldWidth =
            STEP * 2;

        const uint8_t totalWidth =
            fieldWidth +
            gap +
            fieldWidth;

        const int16_t startX =
            (canvas.width() -
             totalWidth) / 2;


        //--------------------------------------------------
        // HORA
        //--------------------------------------------------

        drawTwoDigits(
            canvas,
            editingDateTime.hour,
            startX,
            y);


        //--------------------------------------------------
        // MINUTO
        //--------------------------------------------------

        const int16_t minuteX =
            startX +
            fieldWidth +
            gap;

        drawTwoDigits(
            canvas,
            editingDateTime.minute,
            minuteX,
            y);


        //--------------------------------------------------
        // CURSOR
        //--------------------------------------------------

        int16_t cursorX =
            startX;


        if (currentState ==
            State::EditingMinute)
        {
            cursorX =
                minuteX;
        }


        if (currentState ==
                State::EditingHour ||
            currentState ==
                State::EditingMinute)
        {
            for (uint8_t i = 0;
                 i < fieldWidth - S;
                 i++)
            {
                if ((cursorX + i) >= 0 &&
                    (cursorX + i) <
                        canvas.width())
                {
                    canvas.setPixel(
                        cursorX + i,
                        cursorY,
                        true);
                }
            }
        }

        return;
    }


    //------------------------------------------------------
    // CONFIRMAÇÃO
    //------------------------------------------------------

    if (currentState ==
        State::Confirm)
    {
        /*
         * Não vamos colocar "CONFIRMA"
         * porque não cabe na fonte 5x3.
         *
         * Mostramos somente:
         *
         *          OK
         */

        const char *text =
            "OK";

        drawSmallText(
            canvas,
            text,
            1);

        return;
    }
}
    //------------------------------------------------------
    // Estado
    //------------------------------------------------------

    State state()
    {
        return currentState;
    }

    //------------------------------------------------------
    // Resultado
    //------------------------------------------------------

    bool readResult(
        Result &result)
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

    //------------------------------------------------------
    // Data/hora em edição
    //------------------------------------------------------

    const RtcDateTime &dateTime()
    {
        return editingDateTime;
    }
}