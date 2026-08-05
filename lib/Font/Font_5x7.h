#pragma once

#include <Arduino.h>

namespace Font5x7
{

constexpr uint8_t Width   = 5;
constexpr uint8_t Height  = 7;
constexpr uint8_t Spacing = 1;

// espaço
const uint8_t Space[] PROGMEM =
{
    0x00,0x00,0x00,0x00,0x00
};

// :
const uint8_t Colon[] PROGMEM =
{
    0x00,
    0x36,
    0x36,
    0x00,
    0x00
};

// 0
const uint8_t Digit0[] PROGMEM =
{
    0x3E,
    0x51,
    0x49,
    0x45,
    0x3E
};

// 1
const uint8_t Digit1[] PROGMEM =
{
    0x00,
    0x42,
    0x7F,
    0x40,
    0x00
};

// 2
const uint8_t Digit2[] PROGMEM =
{
    0x62,
    0x51,
    0x49,
    0x49,
    0x46
};

// 3
const uint8_t Digit3[] PROGMEM =
{
    0x22,
    0x41,
    0x49,
    0x49,
    0x36
};

// 4
const uint8_t Digit4[] PROGMEM =
{
    0x18,
    0x14,
    0x12,
    0x7F,
    0x10
};

// 5
const uint8_t Digit5[] PROGMEM =
{
    0x2F,
    0x49,
    0x49,
    0x49,
    0x31
};

// 6
const uint8_t Digit6[] PROGMEM =
{
    0x3E,
    0x49,
    0x49,
    0x49,
    0x32
};

// 7
const uint8_t Digit7[] PROGMEM =
{
    0x01,
    0x71,
    0x09,
    0x05,
    0x03
};

// 8
const uint8_t Digit8[] PROGMEM =
{
    0x36,
    0x49,
    0x49,
    0x49,
    0x36
};

// 9
const uint8_t Digit9[] PROGMEM =
{
    0x26,
    0x49,
    0x49,
    0x49,
    0x3E
};

}