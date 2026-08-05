#pragma once

#include <Arduino.h>

class Canvas;

/**
 * ============================================================
 * Display Driver
 * ------------------------------------------------------------
 * Driver de baixo nível do MAX7219.
 *
 * Responsabilidades:
 *   - Inicializar o hardware
 *   - Ajustar brilho
 *   - Limpar display
 *   - Transferir o Canvas para o MAX7219
 *
 * Não conhece:
 *   - RTC
 *   - Fontes
 *   - Menus
 *   - Relógio
 * ============================================================
 */
class Display
{
public:

    Display();

    bool begin();

    void clear();

    void refresh(const Canvas& canvas);

    void setBrightness(uint8_t level);

    uint8_t brightness() const;

private:

    uint8_t m_brightness;
};

extern Display display;