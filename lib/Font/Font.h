#pragma once

#include <Arduino.h>

namespace Font
{
    // Dimensões da fonte atual
    uint8_t width();
    uint8_t height();

    // Espaçamento entre caracteres
    uint8_t spacing();

    // Retorna ponteiro para o bitmap do caractere
    const uint8_t* glyph(char c);
}