#pragma once

#include <Arduino.h>
#include <MD_MAX72xx.h>

namespace Config
{
    constexpr char FW_NAME[] = "ClockNano";
    constexpr char FW_VERSION[] = "2.0.0";

    constexpr uint8_t DisplayModules = 4;

    constexpr uint8_t PinDIN = 11;
    constexpr uint8_t PinCLK = 13;
    constexpr uint8_t PinCS  = 10;

    constexpr uint8_t BtnMinus = 2;
    constexpr uint8_t BtnOk    = 3;
    constexpr uint8_t BtnPlus  = 4;

    constexpr uint8_t Buzzer = 5;

    constexpr uint8_t DisplayWidth  = 32;
    constexpr uint8_t DisplayHeight = 8;

    constexpr auto Hardware = MD_MAX72XX::FC16_HW;
}