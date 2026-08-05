#pragma once

#include <Arduino.h>

/**
 * @brief Framebuffer monocromático de 32x8 pixels.
 *
 * Cada coluna é armazenada em um byte.
 *
 * bit0 = linha inferior
 * bit7 = linha superior
 *
 * O Canvas não conhece o hardware.
 * Ele apenas mantém uma imagem em memória.
 */
class Canvas
{
public:

    static constexpr uint8_t Width  = 32;
    static constexpr uint8_t Height = 8;

    Canvas();

    /// Limpa todo o framebuffer
    void clear();

    /// Acende todos os pixels
    void fill();

    /// Liga ou desliga um pixel
    void setPixel(
        uint8_t x,
        uint8_t y,
        bool state = true);

    /// Alterna um pixel
    void togglePixel(
        uint8_t x,
        uint8_t y);

    /// Lê um pixel
    bool getPixel(
        uint8_t x,
        uint8_t y) const;

    /// Limpa uma coluna
    void clearColumn(
        uint8_t column);

    /// Escreve uma coluna inteira
    void setColumn(
        uint8_t column,
        uint8_t value);

    /// Lê uma coluna
    uint8_t getColumn(
        uint8_t column) const;

    /// Retorna ponteiro para o framebuffer
    const uint8_t* data() const;

    /// Largura do framebuffer
uint8_t width() const;

/// Altura do framebuffer
uint8_t height() const;

/// Ponteiro para o framebuffer
const uint8_t* frameBuffer() const;

private:

    uint8_t m_buffer[Width];
};

extern Canvas canvas;