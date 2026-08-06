#include <Arduino.h>

#include <Canvas.h>
#include <Display.h>
#include <Graphics.h>

const uint8_t smile[] =
    {
        0b00111100,
        0b01000010,
        0b10100101,
        0b10000001,
        0b10100101,
        0b10011001,
        0b01000010,
        0b00111100};

void setup()
{

display.begin();

canvas.clear();

Graphics::drawStringCentered(
    canvas,
    0,
    "12:34");

display.refresh(canvas);

}

void loop()
{
}
