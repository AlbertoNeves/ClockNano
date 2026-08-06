#include <Arduino.h>

#include <Canvas.h>
#include <Display.h>
#include <ClockView.h>
#include <RTC.h>

//----------------------------------------------------------
// Objetos globais
//----------------------------------------------------------

Canvas canvas;

//----------------------------------------------------------
// Protótipos
//----------------------------------------------------------

static void setupHardware();
static void updateClock();

//----------------------------------------------------------
// setup()
//----------------------------------------------------------

void setup()
{
    setupHardware();
}

//----------------------------------------------------------
// loop()
//----------------------------------------------------------

void loop()
{
    updateClock();
}

//----------------------------------------------------------
// Inicialização do hardware
//----------------------------------------------------------

static void setupHardware()
{
    display.begin();

    RTC::begin();

    canvas.clear();
}

//----------------------------------------------------------
// Atualização do relógio
//----------------------------------------------------------

static void updateClock()
{
    static uint8_t lastSecond = 255;

    RtcDateTime rtcNow;

    if (!RTC::read(rtcNow))
        return;

    if (rtcNow.second == lastSecond)
        return;

    lastSecond = rtcNow.second;

    canvas.clear();

    ClockView::draw(
        canvas,
        rtcNow.hour,
        rtcNow.minute,
        (rtcNow.second & 1) == 0);

    display.refresh(canvas);
}