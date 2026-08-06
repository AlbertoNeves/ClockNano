#include "ClockView.h"

#include <Canvas.h>
#include <Graphics.h>

#include <stdio.h>

namespace ClockView
{

void draw(
    Canvas& canvas,
    uint8_t hour,
    uint8_t minute,
    bool showColon)
{
    char txt[6];

    snprintf(
        txt,
        sizeof(txt),
        "%02u:%02u",
        hour,
        minute);

    if (!showColon)
        txt[2] = ' ';

    Graphics::drawStringCentered(
        canvas,
        0,
        txt);
}

}