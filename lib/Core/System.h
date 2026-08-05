#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

struct DateTimeData
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;

    uint8_t day;
    uint8_t month;
    uint16_t year;
};

struct SettingsData
{
    uint8_t font;
    bool format12h;
    bool beep;
};

struct SystemData
{
    DateTimeData time;

    SettingsData settings;

    bool rtcPresent;
};

extern SystemData sys;

#endif