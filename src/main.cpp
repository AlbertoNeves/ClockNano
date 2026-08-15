#include <Arduino.h>
#include <Buttons.h>
#include <Canvas.h>
#include <Display.h>
#include <ClockView.h>
#include <RTC.h>
#include <Menu.h>
#include <ConfigEEPROM.h>
#include <Alarm.h>

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

    ClockView::setFont(ConfigEEPROM::loadFont());

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
        "FONTE",
        {true,
         0,
         2,
         1});

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
        MenuItemId::About,
        "SOBRE");
}

//----------------------------------------------------------
// Atualização do relógio
//----------------------------------------------------------

static void updateClock()
{
    static uint8_t lastSecond = 255;
    static uint8_t lastColonPhase = 255;
    static uint32_t lastFrame = 0;

    RtcDateTime rtcNow;

    if (!RTC::read(rtcNow))
        return;

    const bool secondsMode =
        (ClockView::font() == ClockFontStyle::Seconds);

    const uint32_t now = millis();
    // Cada fase dura 500 ms: o ':' alterna ligado/desligado duas vezes por segundo.
    const uint8_t colonPhase = (now / 500U) & 1U;

    // No modo SEGUNDOS precisamos redesenhar durante a rolagem.
    // Nos demais modos continuamos atualizando apenas quando o segundo muda.
    if (!secondsMode)
    {
        if (rtcNow.second == lastSecond && colonPhase == lastColonPhase)
            return;

        lastSecond = rtcNow.second;
        lastColonPhase = colonPhase;
    }
    else
    {
        if (rtcNow.second == lastSecond &&
            (now - lastFrame) < 25)
            return;

        lastSecond = rtcNow.second;
        lastColonPhase = colonPhase;
        lastFrame = now;
    }

    canvas.clear();

    ClockView::draw(
        canvas,
        rtcNow.hour,
        rtcNow.minute,
        rtcNow.second,
        colonPhase == 0);

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
    // verifica se algum alarme disparou
    Alarm::service();

    //------------------------------------------------------
    // Teste da camada de edição
    //------------------------------------------------------

    MenuResult result;

    if (Menu::readResult(result))
    {
        //------------------------------------------------------
        // FONTE
        //------------------------------------------------------

        if (result.item == MenuItemId::Font)
        {
            if (result.type == MenuResultType::Selected)
            {
                Menu::setValue(
                    static_cast<int16_t>(ConfigEEPROM::loadFont()));
            }
            else if (result.type == MenuResultType::Changed)
            {
                ClockView::setFont(
                    static_cast<ClockFontStyle>(result.value));
            }
            else if (result.type == MenuResultType::Confirmed)
            {
                ClockFontStyle style =
                    static_cast<ClockFontStyle>(result.value);

                ConfigEEPROM::saveFont(style);
                ClockView::setFont(style);

                Serial.print("FONTE SALVA: ");
                Serial.println(result.value);
            }
        }

        //------------------------------------------------------
        // Hardware - BRILHO
        //------------------------------------------------------

        if (result.item == MenuItemId::Brightness)
        {
            if (result.type == MenuResultType::Selected)
            {
                Menu::setValue(
                    static_cast<int16_t>(ConfigEEPROM::loadBrightness()));
            }

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

    uint8_t alarmIndex;

    if (Alarm::consumeTrigger(alarmIndex))
    {
        Serial.print("ALARM TRIGGER: ");
        Serial.println(alarmIndex + 1);
    }
}
