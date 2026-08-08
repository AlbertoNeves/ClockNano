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
    const uint8_t *glyph(char c);
    
// Dimensões da fonte pequena
    uint8_t smallWidth();
    uint8_t smallHeight();
    uint8_t smallSpacing();

    const uint8_t *smallGlyph(char c);
}