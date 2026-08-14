#include "ConfigEEPROM.h"

#include <EEPROM.h>

namespace ConfigEEPROM
{
    uint8_t loadBrightness()
    {
        uint8_t value = EEPROM.read(ADDR_BRIGHTNESS);
        if (value > 3)
            value = DEFAULT_BRIGHTNESS;
        return value;
    }

    void saveBrightness(uint8_t value)
    {
        if (value > 3)
            value = 3;
        EEPROM.update(ADDR_BRIGHTNESS, value);
    }

    ClockFontStyle loadFont()
    {
        uint8_t value = EEPROM.read(ADDR_FONT);
        if (value > static_cast<uint8_t>(ClockFontStyle::Seconds))
            return DEFAULT_FONT;
        return static_cast<ClockFontStyle>(value);
    }

    void saveFont(ClockFontStyle value)
    {
        uint8_t raw = static_cast<uint8_t>(value);
        if (raw > static_cast<uint8_t>(ClockFontStyle::Seconds))
            raw = static_cast<uint8_t>(DEFAULT_FONT);
        EEPROM.update(ADDR_FONT, raw);
    }
}
