#include <Arduino.h>
#include <Buttons.h>
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
    Serial.begin(115200);
    setupHardware();
}

//----------------------------------------------------------
// Inicialização do hardware
//----------------------------------------------------------

static void setupHardware()
{
    display.begin();

    RTC::begin();

    canvas.clear();

    Buttons::begin();
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
//==========================================================
//           testButtons()
//==========================================================
void testButtons()
{
    ButtonEvent event;

    if (Buttons::read(event))
    {
        Serial.print("Button: ");

        switch (event.button)
        {
        case ButtonId::Minus:

            Serial.print("Minus");
            break;

        case ButtonId::Ok:

            Serial.print("OK");
            break;

        case ButtonId::Plus:

            Serial.print("Plus");
            break;

        default:

            Serial.print("None");
            break;
        }

        Serial.print("  Event: ");

        switch (event.type)
        {
        case ButtonEventType::Click:

            Serial.println("Click");
            break;

        case ButtonEventType::LongPress:

            Serial.println("LongPress");
            break;

        case ButtonEventType::Repeat:

            Serial.println("Repeat");
            break;

        case ButtonEventType::Release:

            Serial.println("Release");
            break;

        default:

            Serial.println("None");
            break;
        }
    }
}

//----------------------------------------------------------
// loop()
//----------------------------------------------------------

void loop()
{
    updateClock();
    Buttons::update();
      testButtons();
}