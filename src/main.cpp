#include <Arduino.h>

#include <Canvas.h>
#include <Display.h>

void setup()
{
    display.begin();

    canvas.clear();

    // Acende alguns pixels para teste
    canvas.setPixel(0,0);
    canvas.setPixel(31,0);

    canvas.setPixel(0,7);
    canvas.setPixel(31,7);

    canvas.setPixel(15,3);
    canvas.setPixel(16,4);

    display.refresh(canvas);
}

void loop()
{
}