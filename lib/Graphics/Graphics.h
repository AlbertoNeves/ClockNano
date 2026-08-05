#pragma once

#include <Arduino.h>

class Canvas;

class Graphics
{
public:

    static void drawPixel(
        Canvas& canvas,
        uint8_t x,
        uint8_t y,
        bool state = true);

    static void drawHLine(
        Canvas& canvas,
        uint8_t x,
        uint8_t y,
        uint8_t length);

    static void drawVLine(
        Canvas& canvas,
        uint8_t x,
        uint8_t y,
        uint8_t length);

    static void drawRectangle(
        Canvas& canvas,
        uint8_t x,
        uint8_t y,
        uint8_t width,
        uint8_t height);

    static void fillRectangle(
        Canvas& canvas,
        uint8_t x,
        uint8_t y,
        uint8_t width,
        uint8_t height);
};