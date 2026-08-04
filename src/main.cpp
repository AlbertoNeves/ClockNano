#include <Arduino.h>

#include "Display.h"

void setup()
{
    display.begin();
}

void loop()
{
    display.showTime(12,45);

    display.update();
}