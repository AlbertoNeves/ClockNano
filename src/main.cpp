#include <Arduino.h>
#include <Buttons.h>
#include <Canvas.h>
#include <Display.h>
#include <ClockView.h>
#include <RTC.h>
#include <Menu.h>
#include <ConfigEEPROM.h>

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

    uint8_t brightness =

        ConfigEEPROM::loadBrightness();

    Serial.print("BRILHO LIDO DA EEPROM: ");
    Serial.println(brightness);

    display.setBrightness(brightness);

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
         3,
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
        // Hardware - BRILHO
        //------------------------------------------------------

        if (result.item == MenuItemId::Brightness)
        {
            uint8_t brightness =
                static_cast<uint8_t>(result.value);

            //------------------------------------------------------
            // Alteração temporária
            //------------------------------------------------------

            if (result.type == MenuResultType::Changed)
            {
                display.setBrightness(brightness);
            }

            //------------------------------------------------------
            // Confirmação
            //------------------------------------------------------

            else if (result.type == MenuResultType::Confirmed)
            {
                ConfigEEPROM::saveBrightness(brightness);

                display.setBrightness(brightness);

                Serial.print("BRILHO SALVO: ");
                Serial.println(brightness);
            }
        }
    }
    //------------------------------------------------------
    // Menu
    //------------------------------------------------------

    if (Menu::state() != MenuState::Closed)
    {
        static uint32_t lastMenuFrame = 25;

        uint32_t now = millis();

        //--------------------------------------------------
        // Atualização do frame
        //--------------------------------------------------

        if ((now - lastMenuFrame) >= 10)
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