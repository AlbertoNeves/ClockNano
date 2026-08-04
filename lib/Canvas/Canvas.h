#ifndef CANVAS_H
#define CANVAS_H

#include <Arduino.h>

class Canvas
{
public:

    static const uint8_t WIDTH = 32;
    static const uint8_t HEIGHT = 8;

    void begin();

    void clear();

    void setPixel(uint8_t x,
                  uint8_t y,
                  bool state = true);

    bool getPixel(uint8_t x,
                  uint8_t y) const;

    void invertPixel(uint8_t x,
                     uint8_t y);

    uint8_t getColumn(uint8_t x) const;

private:

    uint8_t buffer[WIDTH];
};

extern Canvas canvas;

#endif