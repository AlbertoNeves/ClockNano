#pragma once

#include <Arduino.h>

class Canvas;

class Graphics
{
public:
    //======================================================
    // Pixel
    //======================================================

    static void drawPixel(
        Canvas &canvas,
        uint8_t x,
        uint8_t y,
        bool state = true);

    //======================================================
    // Linhas
    //======================================================

    static void drawHLine(
        Canvas &canvas,
        uint8_t x,
        uint8_t y,
        uint8_t length);

    static void drawVLine(
        Canvas &canvas,
        uint8_t x,
        uint8_t y,
        uint8_t length);

    //======================================================
    // Retângulos
    //======================================================

    static void drawRectangle(
        Canvas &canvas,
        uint8_t x,
        uint8_t y,
        uint8_t width,
        uint8_t height);

    static void fillRectangle(
        Canvas &canvas,
        uint8_t x,
        uint8_t y,
        uint8_t width,
        uint8_t height);

    //======================================================
    // Bitmaps
    //======================================================

    static void drawBitmap(
        Canvas &canvas,
        uint8_t x,
        uint8_t y,
        const uint8_t *bitmap,
        uint8_t width,
        uint8_t height,
        bool progmem = false);

    //======================================================
    // Texto
    //======================================================

    static void drawChar(
        Canvas &canvas,
        uint8_t x,
        uint8_t y,
        char c);

    static void drawString(
        Canvas &canvas,
        uint8_t x,
        uint8_t y,
        const char *text);

    static void drawStringCentered(
        Canvas &canvas,
        uint8_t y,
        const char *text);

    //======================================================
    // Utilidades
    //======================================================

    static uint8_t charWidth();

    static uint8_t textWidth(
        const char *text);

    static uint8_t centerX(
        const Canvas &canvas,
        const char *text);
};