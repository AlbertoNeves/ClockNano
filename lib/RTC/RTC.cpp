#include "RTC.h"

#include <RTClib.h>

namespace
{
    RTC_DS3231 rtcDevice;
}

namespace RTC
{

bool begin()
{
    return rtcDevice.begin();
}

bool isRunning()
{
    return !rtcDevice.lostPower();
}

bool read(RtcDateTime& dt)
{
    DateTime rtcNow = rtcDevice.now();

    dt.year      = rtcNow.year();
    dt.month     = rtcNow.month();
    dt.day       = rtcNow.day();

    dt.hour      = rtcNow.hour();
    dt.minute    = rtcNow.minute();
    dt.second    = rtcNow.second();

    dt.dayOfWeek = rtcNow.dayOfTheWeek();

    return true;
}

RtcDateTime now()
{
    RtcDateTime dt;

    read(dt);

    return dt;
}

bool adjust(const RtcDateTime& dt)
{
    rtcDevice.adjust(
        DateTime(
            dt.year,
            dt.month,
            dt.day,
            dt.hour,
            dt.minute,
            dt.second));

    return true;
}

}   // namespace RTC