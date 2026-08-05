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
    0b00111100
};

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

    canvas.clear();

    delay(500);

Graphics::drawRectangle(
    canvas,
    0,
    0,
    32,
    8);

display.refresh(canvas);

delay(500);

canvas.clear();

Graphics::drawRectangle(
    canvas,
    8,
    2,
    16,
    4);

display.refresh(canvas);

delay(500);

canvas.clear();

Graphics::fillRectangle(
    canvas,
    8,
    2,
    16,
    4);

display.refresh(canvas);

delay(500);

canvas.clear();

Graphics::drawRectangle(
    canvas,
    0,
    0,
    32,
    8);

Graphics::fillRectangle(
    canvas,
    2,
    2,
    28,
    4);

display.refresh(canvas);

delay(500);
canvas.clear();

Graphics::drawBitmap(
    canvas,
    12,
    0,
    smile,
    8,
    8);

display.refresh(canvas);

}

void loop()
{
}

