#include <Arduino.h>

#include <Canvas.h>
#include <Display.h>
#include <Graphics.h>

void setup()
{
    display.begin();

    canvas.clear();

    Graphics::drawPixel(canvas,15,3);

    display.refresh(canvas);
}

void loop()
{
}