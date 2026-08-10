#pragma once

#include <Arduino.h>
#include <Buttons.h>
#include <Canvas.h>

namespace Alarm
{

//==========================================================
// Quantidade de alarmes
//==========================================================

constexpr uint8_t MAX_ALARMS = 3;


//==========================================================
// Tipo de repetição
//==========================================================

enum class Repeat : uint8_t
{
    Once = 0,
    Daily,
    WeekDays
};


//==========================================================
// Dados de um alarme
//==========================================================

struct AlarmData
{
    uint8_t hour;
    uint8_t minute;

    Repeat repeat;

    // Máscara dos dias:
    //
    // bit 0 = SEG
    // bit 1 = TER
    // bit 2 = QUA
    // bit 3 = QUI
    // bit 4 = SEX
    // bit 5 = SAB
    // bit 6 = DOM

    uint8_t weekDays;

    // Índice da melodia
    uint8_t melody;

    // Alarme habilitado
    bool enabled;
};


//==========================================================
// Estado da edição
//==========================================================

enum class State : uint8_t
{
    Inactive = 0,

    SelectingAlarm,

    EditingRepeat,
    EditingHour,
    EditingMinute,
    EditingWeekDays
};


//==========================================================
// Resultado da edição
//==========================================================

enum class Result : uint8_t
{
    None = 0,
    Changed,
    FieldConfirmed,
    Confirmed
};


//==========================================================
// Inicialização
//==========================================================

void begin();


//==========================================================
// Inicia edição
//
// Sem argumento:
//     mantém compatibilidade com Menu.cpp atual
//     e edita o alarme 0.
//
// Com argumento:
//     seleciona qual dos 3 alarmes será editado.
//==========================================================

void start();

void start(
    uint8_t index);


//==========================================================
// Atualiza máquina de estados
//==========================================================

void update(
    const ButtonEvent &event);


//==========================================================
// Renderiza tela
//==========================================================

void draw(
    Canvas &canvas);


//==========================================================
// Estado atual
//==========================================================

State state();


//==========================================================
// Resultado pendente
//==========================================================

bool readResult(
    Result &result);


//==========================================================
// Alarme atualmente em edição
//==========================================================

uint8_t currentAlarm();


//==========================================================
// Valores do alarme atualmente em edição
//
// Mantemos estas funções para não quebrar o Menu.cpp.
//==========================================================

uint8_t hour();

uint8_t minute();

Repeat repeat();

uint8_t weekDays();


//==========================================================
// Acesso aos três alarmes
//==========================================================

uint8_t hour(
    uint8_t index);

uint8_t minute(
    uint8_t index);

Repeat repeat(
    uint8_t index);

uint8_t weekDays(
    uint8_t index);

uint8_t melody(
    uint8_t index);

bool enabled(
    uint8_t index);


//==========================================================
// Controle de habilitação
//==========================================================

void setEnabled(
    uint8_t index,
    bool value);


//==========================================================
// Cancela edição
//==========================================================

void cancel();

} // namespace Alarm