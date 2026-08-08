#include <Arduino.h>
#include <Buttons.h>
#include <Canvas.h>
#include <Display.h>
#include <ClockView.h>
#include <RTC.h>
#include <Menu.h>

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

    Menu::begin();

    Menu::addItem(
        MenuItemId::Time,
        "HORA");

    Menu::addItem(
        MenuItemId::Date,
        "DATA");

    Menu::addItem(
        MenuItemId::Font,
        "FONTE");

    Menu::addItem(
        MenuItemId::Alarm,
        "ALARME");

    Menu::addItem(
        MenuItemId::Brightness,
        "BRILHO",
        {true,
         0,
         15,
         1});

    Menu::addItem(
        MenuItemId::Contrast,
        "CONTRASTE");

    Menu::addItem(
        MenuItemId::About,
        "SOBRE");
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

//----------------------------------------------------------
// loop()
//----------------------------------------------------------

void loop()
{
    //------------------------------------------------------
    // Botões
    //------------------------------------------------------

    Buttons::update();

    //------------------------------------------------------
    // Consumir evento
    //------------------------------------------------------

    ButtonEvent event;

    if (Buttons::read(event))
    {
        Menu::update(event);
    }

    //------------------------------------------------------
    // Teste da camada de edição
    //------------------------------------------------------

    MenuResult result;

if (Menu::readResult(result))
{
    //------------------------------------------------------
    // Debug
    //------------------------------------------------------

    Serial.print("MenuResult: ");

    Serial.print(
        static_cast<uint8_t>(result.type));

    Serial.print("  Item: ");

    Serial.print(
        static_cast<uint8_t>(result.item));

    Serial.print("  Valor: ");

    Serial.println(result.value);


    //------------------------------------------------------
    // Hardware - BRILHO
    //------------------------------------------------------

    if (result.item == MenuItemId::Brightness)
    {
        if (result.type == MenuResultType::Changed ||
            result.type == MenuResultType::Confirmed)
        {
            display.setBrightness(
                static_cast<uint8_t>(result.value));
        }
    }
}
    //------------------------------------------------------
    // Menu
    //------------------------------------------------------

    if (Menu::state() != MenuState::Closed)
    {
        static uint32_t lastMenuFrame = 0;

        uint32_t now = millis();

        //--------------------------------------------------
        // Atualização do frame
        //--------------------------------------------------

        if ((now - lastMenuFrame) >= 25)
        {
            lastMenuFrame = now;

            Menu::draw(canvas);

            display.refresh(canvas);
        }

        return;
    }

    //------------------------------------------------------
    // Relógio
    //------------------------------------------------------

    updateClock();
}