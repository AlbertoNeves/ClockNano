#pragma once

#include <Arduino.h>
#include <Buttons.h>
#include <Canvas.h>

namespace Alarm
{
    constexpr uint8_t MAX_ALARMS = 3;

    enum class Repeat : uint8_t
    {
        Once = 0,
        Daily,
        WeekDays
    };

    struct AlarmData
    {
        uint8_t hour;
        uint8_t minute;
        Repeat repeat;
        uint8_t weekDays;
        uint8_t melody;
        bool enabled;
    };

    enum class State : uint8_t
    {
        Inactive = 0,
        SelectingAlarm,
        EditingHour,
        EditingMinute,
        EditingRepeat,
        EditingWeekDays
    };

    enum class Result : uint8_t
    {
        None = 0,
        Changed,
        FieldConfirmed,
        Confirmed
    };

    void begin();

    // Mantida para compatibilidade: inicia o ALRM 1.
    void start();

    // Inicia a edição do alarme 0, 1 ou 2.
    void start(uint8_t index);

    // Atualiza a máquina de estados dos botões.
    void update(const ButtonEvent &event);

    // Renderiza a tela do alarme.
    void draw(Canvas &canvas);

    State state();

    bool readResult(Result &result);

    uint8_t currentAlarm();

    uint8_t hour();
    uint8_t minute();
    Repeat repeat();
    uint8_t weekDays();

    uint8_t hour(uint8_t index);
    uint8_t minute(uint8_t index);
    Repeat repeat(uint8_t index);
    uint8_t weekDays(uint8_t index);
    uint8_t melody(uint8_t index);
    bool enabled(uint8_t index);
    void setEnabled(uint8_t index, bool value);

    // Serviço contínuo: verifica coincidências com o RTC.
    void service();

    // Retorna o índice (0..2) do próximo alarme disparado.
    // Se houver mais de um disparo pendente, eles são consumidos
    // um por vez.
    bool consumeTrigger(uint8_t &index);

    void cancel();
}
