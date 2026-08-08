#pragma once

#include <Arduino.h>
#include <Buttons.h>
#include <Canvas.h>

//==========================================================
// ClockNano
// Camada de edição do ALARME
//
// Nesta primeira etapa:
// - somente hora e minuto;
// - valores somente em RAM;
// - sem DS3231;
// - sem EEPROM;
// - sem repetição.
//
// Interface:
//     + / -  -> altera campo
//     OK     -> confirma campo
//
// Fluxo:
//
//     HORA
//      ↓ OK
//     MINUTO
//      ↓ OK
//     confirmado
//==========================================================

namespace Alarm
{
    //======================================================
    // Estado da edição
    //======================================================

    enum class State : uint8_t
    {
        Inactive = 0,
        EditingHour,
        EditingMinute
    };

    //======================================================
    // Resultado da edição
    //======================================================

    enum class Result : uint8_t
    {
        None = 0,
        Changed,
        HourConfirmed,
        Confirmed
    };

    //======================================================
    // Inicialização
    //======================================================

    void begin();

    //======================================================
    // Inicia edição do alarme
    //
    // Sempre começa em 00:00 nesta primeira etapa.
    //======================================================

    void start();

    //======================================================
    // Atualiza a máquina de estados
    //======================================================

    void update(
        const ButtonEvent& event);

    //======================================================
    // Renderiza a tela
    //======================================================

    void draw(
        Canvas& canvas);

    //======================================================
    // Retorna estado atual
    //======================================================

    State state();

    //======================================================
    // Retorna resultado pendente
    //======================================================

    bool readResult(
        Result& result);

    //======================================================
    // Retorna hora atual da edição
    //======================================================

    uint8_t hour();

    //======================================================
    // Retorna minuto atual da edição
    //======================================================

    uint8_t minute();

    //======================================================
    // Encerra edição
    //======================================================

    void cancel();
}