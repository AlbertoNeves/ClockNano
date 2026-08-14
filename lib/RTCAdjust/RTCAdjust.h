#ifndef RTC_ADJUST_H
#define RTC_ADJUST_H

#include <stdint.h>
#include <RTC.h>
#include <Buttons.h>
#include <Canvas.h>

namespace RTCAdjust
{
    enum class State : uint8_t
    {
        Inactive = 0,

        EditingYear,
        EditingMonth,
        EditingDay,
        EditingHour,
        EditingMinute,

        Confirm
    };

    enum class Result : uint8_t
    {
        None = 0,
        Changed,
        FieldConfirmed,
        Confirmed,
        Cancelled
    };

    enum class Mode : uint8_t
    {
        Date,
        Time
    };

    void begin();

    void start(Mode mode);

    void update(
        const ButtonEvent &event);

    void draw(
        Canvas &canvas);

    State state();

    bool readResult(
        Result &result);

    const RtcDateTime &dateTime();
}

#endif