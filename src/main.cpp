#include <Arduino.h>

#include <Canvas.h>
#include <Display.h>
#include <Graphics.h>

void setup()
{
    display.begin();

    canvas.clear();

    Graphics::drawHLine(canvas, 0, 0, 32);

    Graphics::drawHLine(canvas, 0, 7, 32);

    display.refresh(canvas);

delay(1000);
    canvas.clear();

Graphics::drawHLine(canvas, 5, 3, 20);

display.refresh(canvas);

delay(1000);
    canvas.clear();

    for (size_t j = 0; j < 9; j++)
    {
      for (size_t i = 0; i < 33; i++)
    {
      Graphics::drawHLine(canvas,j, j, i);
       display.refresh(canvas);
    }
    }
    canvas.clear();
    
    
}

void loop()
{
}