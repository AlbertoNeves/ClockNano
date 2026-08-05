#include <Arduino.h>

#include <Canvas.h>
#include <Display.h>
#include <Graphics.h>

void setup()
{
    display.begin();

    canvas.clear();

    Graphics::drawVLine(canvas, 0, 0, 8);

    Graphics::drawVLine(canvas, 31, 0, 8);

    display.refresh(canvas);

    delay(500);

    canvas.clear();

    Graphics::drawHLine(canvas, 0, 3, 32);

    Graphics::drawVLine(canvas, 15, 0, 8);

    display.refresh(canvas);
}

void loop()
{
}

