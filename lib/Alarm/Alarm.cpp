#include "Alarm.h"

#include <Font.h>
#include <avr/pgmspace.h>

namespace
{
    //======================================================
    // Dados dos 3 alarmes
    //======================================================

    Alarm::AlarmData alarms[Alarm::MAX_ALARMS] =
        {
            {
                0,                   // hour
                0,                   // minute
                Alarm::Repeat::Once, // repeat
                0,                   // weekDays
                0,                   // melody
                false                // enabled
            },

            {0,
             0,
             Alarm::Repeat::Once,
             0,
             0,
             false},

            {0,
             0,
             Alarm::Repeat::Once,
             0,
             0,
             false}};

    //======================================================
    // Alarme atualmente em edição
    //======================================================

    uint8_t editingAlarmIndex = 0;

    // Máscara dos dias da semana
    //
    // bit 0 = SEG
    // bit 1 = TER
    // bit 2 = QUA
    // bit 3 = QUI
    // bit 4 = SEX
    // bit 5 = SAB
    // bit 6 = DOM

    uint8_t alarmDayCursor = 0;
    //======================================================
    // Evento de disparo do alarme'
    //======================================================

    bool alarmTriggered = false;

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
    // Próximo alarme
    //======================================================

    void nextAlarm()
    {
        editingAlarmIndex++;

        if (editingAlarmIndex >=
            Alarm::MAX_ALARMS)
        {
            editingAlarmIndex = 0;
        }
    }

    //======================================================
    // Alarme anterior
    //======================================================

    void previousAlarm()
    {
        if (editingAlarmIndex == 0)
        {
            editingAlarmIndex =
                Alarm::MAX_ALARMS - 1;
        }
        else
        {
            editingAlarmIndex--;
        }
    }
    //======================================================
    // Incrementa hora
    //======================================================
    void increaseHour()
    {
        Alarm::AlarmData &alarm =
            alarms[editingAlarmIndex];

        alarm.hour++;

        if (alarm.hour > 23)
            alarm.hour = 0;
    }
    //======================================================
    // Decrementa hora
    //======================================================
    void decreaseHour()
    {
        Alarm::AlarmData &alarm =
            alarms[editingAlarmIndex];

        if (alarm.hour == 0)
            alarm.hour = 23;
        else
            alarm.hour--;
    }

    //======================================================
    // Incrementa minuto
    //======================================================

    void increaseMinute()
    {
        Alarm::AlarmData &alarm =
            alarms[editingAlarmIndex];

        alarm.minute++;

        if (alarm.minute > 59)
            alarm.minute = 0;
    }
    //======================================================
    // Decrementa minuto
    //======================================================

    void decreaseMinute()
    {
        Alarm::AlarmData &alarm =
            alarms[editingAlarmIndex];

        if (alarm.minute == 0)
            alarm.minute = 59;
        else
            alarm.minute--;
    }
    //======================================================
    // Incrementa repetição
    //======================================================

    void increaseRepeat()
    {
        Alarm::AlarmData &alarm =
            alarms[editingAlarmIndex];

        uint8_t value =
            static_cast<uint8_t>(
                alarm.repeat);

        value++;

        if (value > 2)
            value = 0;

        alarm.repeat =
            static_cast<Alarm::Repeat>(value);
    }
    //======================================================
    // Decrementa repetição
    //======================================================

    void decreaseRepeat()
    {
        Alarm::AlarmData &alarm =
            alarms[editingAlarmIndex];

        uint8_t value =
            static_cast<uint8_t>(
                alarm.repeat);

        if (value == 0)
            value = 2;
        else
            value--;

        alarm.repeat =
            static_cast<Alarm::Repeat>(value);
    }

    //======================================================
    // Desenha caractere da fonte pequena
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
    // Desenha dois pontos HH:MM
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
    // Desenha indicador do campo selecionado
    //======================================================

    void drawSelection(
        Canvas &canvas)
    {
        const uint8_t charWidth =
            Font::smallWidth();

        const uint8_t spacing =
            Font::smallSpacing();

        const uint8_t step =
            charWidth + spacing;

        const uint8_t totalWidth =
            (step * 4) + 2;

        const int16_t startX =
            (canvas.width() -
             totalWidth) /
            2;

        const uint8_t y =
            (canvas.height() -
             Font::smallHeight()) /
            2;

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

        for (uint8_t i = 0;
             i < (step * 2) - 1;
             i++)
        {
            if ((x + i) >= 0 &&
                (x + i) < canvas.width())
            {
                canvas.setPixel(
                    x + i,
                    lineY,
                    true);
            }
        }
    }

    //======================================================
    // Desenha tela HH:MM
    //======================================================

    void drawTime(
        Canvas &canvas)
    {
        const uint8_t charWidth =
            Font::smallWidth();

        const uint8_t spacing =
            Font::smallSpacing();

        const uint8_t step =
            charWidth + spacing;

        const uint8_t totalWidth =
            (step * 4) + 2;

        const int16_t startX =
            (canvas.width() -
             totalWidth) /
            2;

        const uint8_t y =
            (canvas.height() -
             Font::smallHeight()) /
            2;

        // Hora

        drawTwoDigits(
            canvas,
            alarms[editingAlarmIndex].hour,
            startX,
            y);

        // :

        const int16_t colonX =
            startX +
            (step * 2);

        drawColon(
            canvas,
            colonX,
            y + 1);

        // Minuto

        const int16_t minuteX =
            colonX + 2;

        drawTwoDigits(
            canvas,
            alarms[editingAlarmIndex].minute,
            minuteX,
            y);

        // Indicador

        drawSelection(canvas);
    }
    //======================================================
    //======================================================
    // Alterna seleção do dia atual
    //======================================================

    void toggleWeekDay()
    {
        alarms[editingAlarmIndex].weekDays ^=
            (1 << alarmDayCursor);
    }

    //======================================================
    // Próximo dia
    //======================================================

    void nextWeekDay()
    {
        alarmDayCursor++;

        if (alarmDayCursor > 6)
            alarmDayCursor = 0;
    }

    //======================================================
    // Dia anterior
    //======================================================

    void previousWeekDay()
    {
        if (alarmDayCursor == 0)
            alarmDayCursor = 6;
        else
            alarmDayCursor--;
    }

    //======================================================
    // Desenha tela PERSONAL
    //
    //        S T Q Q S S D
    //        ─
    //======================================================

    void drawWeekDays(
        Canvas &canvas)
    {
        static const char days[] =
            {
                'S', 'T', 'Q', 'Q', 'S', 'S', 'D'};

        const uint8_t charWidth =
            Font::smallWidth();

        const uint8_t spacing =
            Font::smallSpacing();

        const uint8_t step =
            charWidth + spacing;

        const uint8_t totalWidth =
            (step * 7) - spacing;

        const int16_t startX =
            (canvas.width() - totalWidth) / 2;

        const uint8_t y =
            (canvas.height() -
             Font::smallHeight()) /
            2;

        //--------------------------------------------------
        // Letras
        //--------------------------------------------------

        for (uint8_t i = 0; i < 7; i++)
        {
            const int16_t x =
                startX + (i * step);

            drawSmallChar(
                canvas,
                days[i],
                x,
                y);
        }

        //--------------------------------------------------
        // Cursor
        //--------------------------------------------------

        const int16_t cursorX =
            startX +
            (alarmDayCursor * step);

        const uint8_t cursorY =
            y + Font::smallHeight();

        for (uint8_t i = 0;
             i < charWidth;
             i++)
        {
            if ((cursorX + i) >= 0 &&
                (cursorX + i) < canvas.width() &&
                cursorY < canvas.height())
            {
                canvas.setPixel(
                    cursorX + i,
                    cursorY,
                    true);
            }
        }

        //--------------------------------------------------
        // Dias selecionados
        //--------------------------------------------------

        const uint8_t selectedY =
            cursorY + 1;

        for (uint8_t day = 0;
             day < 7;
             day++)
        {
            if (alarms[editingAlarmIndex].weekDays &
                (1 << day))
            {
                const int16_t x =
                    startX +
                    (day * step) +
                    1;

                if (selectedY < canvas.height())
                {
                    canvas.setPixel(
                        x,
                        selectedY,
                        true);
                }
            }
        }
    }
    //======================================================
    // Desenha tela de repetição
    //======================================================
    void drawRepeat(
        Canvas &canvas)
    {
        const uint8_t y =
            (canvas.height() -
             Font::smallHeight()) /
            2;

        switch (
            alarms[editingAlarmIndex].repeat)
        {
        case Alarm::Repeat::Once:

            drawSmallText(
                canvas,
                "UNICO",
                y);

            break;

        case Alarm::Repeat::Daily:

            drawSmallText(
                canvas,
                "DIARIO",
                y);

            break;

        case Alarm::Repeat::WeekDays:

            drawSmallText(
                canvas,
                "PERSONAL",
                y);

            break;
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
        for (uint8_t i = 0;
             i < MAX_ALARMS;
             i++)
        {
            alarms[i].hour = 0;
            alarms[i].minute = 0;

            alarms[i].repeat =
                Repeat::Once;

            alarms[i].weekDays = 0;

            alarms[i].melody = 0;

            alarms[i].enabled = false;
        }

        editingAlarmIndex = 0;

        alarmDayCursor = 0;

        currentState =
            State::Inactive;

        clearResult();
    }

    //======================================================
    // Inicia edição
    //======================================================
    void start()
    {
        start(0);
    }

    void start(
        uint8_t index)
    {
        if (index >= MAX_ALARMS)
            return;

        editingAlarmIndex = index;

        alarmDayCursor = 0;

        currentState =
            State::SelectingAlarm;

        clearResult();
    }

    //======================================================
    // Atualização
    //======================================================

    void update(
        const ButtonEvent &event)
    {
        if (currentState ==
            State::Inactive)
        {
            return;
        }
        //======================================================
        // SELEÇÃO DO ALARME
        //
        // + / - : seleciona ALRM 1 / 2 / 3
        // OK    : confirma
        // HOME  : sai
        //======================================================

        if (currentState ==
            State::SelectingAlarm)
        {
            //--------------------------------------------------
            // +
            //--------------------------------------------------

            if (event.button ==
                ButtonId::Plus)
            {
                if (event.type ==
                        ButtonEventType::Click ||
                    event.type ==
                        ButtonEventType::Repeat)
                {
                    nextAlarm();

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
                    previousAlarm();

                    createResult(
                        Result::Changed);
                }

                return;
            }

            //--------------------------------------------------
            // OK
            //--------------------------------------------------

            if (event.button ==
                ButtonId::Ok)
            {
                if (event.type ==
                    ButtonEventType::Click)
                {
                    currentState =
                        State::EditingRepeat;

                    createResult(
                        Result::FieldConfirmed);
                }

                return;
            }

            //--------------------------------------------------
            // HOME
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
                        Result::Confirmed);
                }

                return;
            }
        }
        //==================================================
        // HORA
        //==================================================

        if (currentState ==
            State::EditingHour)
        {
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

            if (event.button ==
                ButtonId::Ok)
            {
                if (event.type ==
                    ButtonEventType::Click)
                {
                    currentState =
                        State::EditingMinute;

                    createResult(
                        Result::FieldConfirmed);
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

            if (event.button ==
                ButtonId::Ok)
            {
                if (event.type ==
                    ButtonEventType::Click)
                {
                    if (alarms[editingAlarmIndex].repeat ==
                        Repeat::WeekDays)
                    {
                        currentState =
                            State::EditingWeekDays;
                    }
                    else
                    {
                        alarms[editingAlarmIndex].enabled =
                            true;

                        currentState =
                            State::Inactive;

                        createResult(
                            Result::Confirmed);

                        return;
                    }

                    createResult(
                        Result::FieldConfirmed);
                }

                return;
            }
        }

        //==================================================
        // REPETIÇÃO
        //==================================================

        if (currentState ==
            State::EditingRepeat)
        {
            //--------------------------------------------------
            // +
            //--------------------------------------------------
            if (event.button ==
                ButtonId::Plus)
            {
                if (event.type ==
                        ButtonEventType::Click ||
                    event.type ==
                        ButtonEventType::Repeat)
                {
                    increaseRepeat();

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
                    decreaseRepeat();

                    createResult(
                        Result::Changed);
                }

                return;
            }
            //--------------------------------------------------
            // OK
            //--------------------------------------------------

            if (event.button ==
                ButtonId::Ok)
            {
                if (event.type ==
                    ButtonEventType::Click)
                {
                    currentState =
                        State::EditingHour;

                    createResult(
                        Result::FieldConfirmed);
                }

                return;
            }
        }

        //==================================================
        // DIAS DA SEMANA - PERSONAL
        //
        // + / - : movimenta o cursor
        // OK    : seleciona / deseleciona o dia
        //==================================================

        if (currentState ==
            State::EditingWeekDays)
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
                    nextWeekDay();

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
                    previousWeekDay();

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
                    toggleWeekDay();

                    createResult(
                        Result::Changed);
                }

                return;
            }
            //------------------------------------------------
            // HOME
            // Confirma PERSONAL
            //------------------------------------------------

            if (event.button ==
                ButtonId::Home)
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
    // Desenha seleção do alarme
    //======================================================

    void drawAlarmSelection(
        Canvas &canvas)
    {
        char text[8];

        text[0] = 'A';
        text[1] = 'L';
        text[2] = 'R';
        text[3] = 'M';
        text[4] = ' ';

        text[5] =
            '1' + editingAlarmIndex;

        text[6] = '\0';

        drawSmallText(
            canvas,
            text,
            (canvas.height() -
             Font::smallHeight()) /
                2);
    }
    //======================================================
    // Renderização
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

        if (currentState ==
            State::SelectingAlarm)
        {
            drawAlarmSelection(canvas);
            return;
        }
        if (currentState ==
                State::EditingHour ||
            currentState ==
                State::EditingMinute)
        {
            drawTime(canvas);
            return;
        }

        if (currentState ==
            State::EditingRepeat)
        {
            drawRepeat(canvas);
            return;
        }

        if (currentState ==
            State::EditingWeekDays)
        {
            drawWeekDays(canvas);
            return;
        }
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

    //======================================================
    // Hora
    //======================================================

    //======================================================
    // Alarme atualmente em edição
    //======================================================

    uint8_t currentAlarm()
    {
        return editingAlarmIndex;
    }

    //======================================================
    // Hora - alarme atual
    //======================================================

    uint8_t hour()
    {
        return alarms[editingAlarmIndex].hour;
    }

    //======================================================
    // Minuto - alarme atual
    //======================================================

    uint8_t minute()
    {
        return alarms[editingAlarmIndex].minute;
    }

    //======================================================
    // Repetição - alarme atual
    //======================================================

    Repeat repeat()
    {
        return alarms[editingAlarmIndex].repeat;
    }

    //======================================================
    // Dias - alarme atual
    //======================================================

    uint8_t weekDays()
    {
        return alarms[editingAlarmIndex].weekDays;
    }

    //======================================================
    // Hora - alarme específico
    //======================================================

    uint8_t hour(
        uint8_t index)
    {
        if (index >= MAX_ALARMS)
            return 0;

        return alarms[index].hour;
    }

    //======================================================
    // Minuto - alarme específico
    //======================================================

    uint8_t minute(
        uint8_t index)
    {
        if (index >= MAX_ALARMS)
            return 0;

        return alarms[index].minute;
    }

    //======================================================
    // Repetição - alarme específico
    //======================================================

    Repeat repeat(
        uint8_t index)
    {
        if (index >= MAX_ALARMS)
            return Repeat::Once;

        return alarms[index].repeat;
    }

    //======================================================
    // Dias - alarme específico
    //======================================================

    uint8_t weekDays(
        uint8_t index)
    {
        if (index >= MAX_ALARMS)
            return 0;

        return alarms[index].weekDays;
    }

    //======================================================
    // Melodia - alarme específico
    //======================================================

    uint8_t melody(
        uint8_t index)
    {
        if (index >= MAX_ALARMS)
            return 0;

        return alarms[index].melody;
    }

    //======================================================
    // Habilitação - alarme específico
    //======================================================

    bool enabled(
        uint8_t index)
    {
        if (index >= MAX_ALARMS)
            return false;

        return alarms[index].enabled;
    }

    //======================================================
    // Define habilitação
    //======================================================

    void setEnabled(
        uint8_t index,
        bool value)
    {
        if (index >= MAX_ALARMS)
            return;

        alarms[index].enabled = value;
    }

    //======================================================
    // Evento de disparo
    //======================================================

    bool consumeTrigger()
    {
        if (!alarmTriggered)
            return false;

        alarmTriggered = false;

        return true;
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