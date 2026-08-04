#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

class Display
{
public:

    bool begin();

    void clear();

    void update();

    void showTime(uint8_t hour,
                  uint8_t minute,
                  bool blink = false);

private:

    void drawDigit(uint8_t x,
                   uint8_t digit);

};

extern Display display;

#endif