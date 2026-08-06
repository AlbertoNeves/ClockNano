#pragma once

#include <Arduino.h>

struct RtcDateTime
{
    uint16_t year;

    uint8_t month;
    uint8_t day;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    uint8_t dayOfWeek;
};

namespace RTC
{
    //------------------------------------------------------
    // Inicialização
    //------------------------------------------------------

    bool begin();

    bool isRunning();

    //------------------------------------------------------
    // Leitura
    //------------------------------------------------------

    bool read(RtcDateTime& dt);

    //------------------------------------------------------
    // Escrita
    //------------------------------------------------------

    bool adjust(const RtcDateTime& dt);

    //------------------------------------------------------
    // Compatibilidade (temporária)
    //------------------------------------------------------

   RtcDateTime now();
}