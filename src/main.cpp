#include <Arduino.h>

#include <Canvas.h>

void setup()
{
    canvas.clear();

    canvas.setPixel(10,3);

    canvas.setPixel(15,7);

    canvas.togglePixel(10,3);
}

void loop()
{
}