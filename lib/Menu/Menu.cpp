#include "Menu.h"

#include <Arduino.h>
#include <Graphics.h>
#include <Font.h>
#include <Alarm.h>
#include <RTCAdjust.h>

#include <avr/pgmspace.h>
#include <string.h>

namespace
{

    //==========================================================
    // Configuração
    //==========================================================

    constexpr uint8_t MaxItems = 8;

    constexpr uint16_t MenuOpenTime = 2000;

    constexpr uint8_t OpenRepeatCount = 11;

    //==========================================================
    // MenuRenderer
    //==========================================================

    constexpr uint8_t AnimationStep = 1;

    constexpr uint16_t AnimationInterval = 20;

    constexpr uint16_t ScrollInterval = 50;

    constexpr uint8_t ScrollGap = 8;

    //==========================================================
    // Máquina de estados do Menu
    //==========================================================

    MenuState currentState =
        MenuState::Closed;

    //==========================================================
    // Itens
    //==========================================================

    MenuItem items[MaxItems];

    uint8_t itemCount = 0;

    uint8_t currentIndex = 0;
    //==========================================================
    // Controle da tela de Alarme
    //==========================================================

    bool alarmActive = false;

    bool aboutActive = false;

    uint8_t aboutScrollCount = 0;

    //==========================================================
    // Controle da tela de Ajuste RTC
    //==========================================================

    bool rtcAdjustActive = false;

    //==========================================================
    // Valor de edição
    //==========================================================

    int16_t currentValue = 0;

    //==========================================================
    // Controle de entrada no Menu
    //==========================================================

    bool okLongPressActive = false;

    uint8_t okRepeatCount = 0;

    //==========================================================
    // Resultado
    //==========================================================

    MenuResult pendingResult =
        {
            MenuResultType::None,
            MenuItemId::None,
            0};
    //==========================================================
    void increaseValue()
    {
        const MenuEditConfig &edit =
            items[currentIndex].edit;

        if (!edit.editable)
            return;

        int16_t next =
            currentValue + edit.step;

        if (next > edit.maximum)
            next = edit.maximum;

        currentValue = next;
    }
    //==========================================================

    void decreaseValue()
    {
        const MenuEditConfig &edit =
            items[currentIndex].edit;

        if (!edit.editable)
            return;

        int16_t next =
            currentValue - edit.step;

        if (next < edit.minimum)
            next = edit.minimum;

        currentValue = next;
    }

    //==========================================================
    // MenuRenderer
    //==========================================================
    //
    // Responsabilidade:
    //
    // - desenhar o item atual;
    // - calcular posição;
    // - executar animação;
    // - não conhece Buttons;
    // - não conhece RTC;
    // - não conhece Display.
    //
    // Usa somente Canvas + Font.
    //

    class MenuRenderer
    {
    public:
        void reset();

        void startNext();

        void startPrevious();

        void update();

        void draw(
            Canvas &canvas,
            const char *text);

        void startScrolling(
            const char *text);

        bool drawScrolling(
            Canvas &canvas,
            const char *text);

        void drawEditing(
            Canvas &canvas,
            const char *text,
            int16_t value);

    private:
        void drawTextClipped(
            Canvas &canvas,
            const char *text,
            int16_t x,
            uint8_t y);

        void drawCharClipped(
            Canvas &canvas,
            char c,
            int16_t x,
            uint8_t y);

        int16_t centeredX(
            const char *text);

    private:
        bool m_animating = false;

        bool m_directionNext = true;

        int16_t m_x = 0;
        int16_t m_targetX = 0;

        uint32_t m_lastStep = 0;

        const char *m_scrollText = nullptr;

        int16_t m_scrollX = 0;

        uint32_t m_lastScrollStep = 0;
    };

    MenuRenderer renderer;

    //==========================================================
    // Auxiliares de resultado
    //==========================================================

    void clearResult()
    {
        pendingResult.type =
            MenuResultType::None;

        pendingResult.item =
            MenuItemId::None;

        pendingResult.value = 0;
    }

    //----------------------------------------------------------

    void createResult(
        MenuResultType type,
        MenuItemId item,
        int16_t value)
    {
        pendingResult.type = type;
        pendingResult.item = item;
        pendingResult.value = value;
    }

    //==========================================================
    // MenuRenderer
    //==========================================================

    void MenuRenderer::reset()
    {
        m_animating = false;

        m_directionNext = true;

        m_x = 0;
        m_targetX = 0;

        m_lastStep = millis();

        m_scrollText = nullptr;

        m_scrollX = 0;

        m_lastScrollStep = millis();
    }

    //----------------------------------------------------------

    int16_t MenuRenderer::centeredX(
        const char *text)
    {
        if (text == nullptr)
            return 0;

        uint8_t width =
            Graphics::textWidth(text);

        if (width >= 32)
            return 0;

        return (32 - width) / 2;
    }

    //----------------------------------------------------------

    void MenuRenderer::startNext()
    {
        m_scrollText = nullptr;

        m_directionNext = true;

        m_animating = true;

        m_targetX = centeredX(
            items[currentIndex].name);

        m_x = m_targetX + 32;

        m_lastStep = millis();
    }

    //----------------------------------------------------------

    void MenuRenderer::startPrevious()
    {
        m_scrollText = nullptr;

        m_directionNext = false;

        m_animating = true;

        m_targetX = centeredX(
            items[currentIndex].name);

        m_x = m_targetX - 32;

        m_lastStep = millis();
    }

    //----------------------------------------------------------

    void MenuRenderer::update()
    {
        if (!m_animating)
            return;

        uint32_t now = millis();

        if ((now - m_lastStep) <
            AnimationInterval)
        {
            return;
        }

        m_lastStep = now;

        //------------------------------------------------------
        // Próximo item
        // Entra pela direita
        //------------------------------------------------------

        if (m_directionNext)
        {
            m_x -= AnimationStep;

            if (m_x <= m_targetX)
            {
                m_x = m_targetX;
                m_animating = false;
            }

            return;
        }

        //------------------------------------------------------
        // Item anterior
        // Entra pela esquerda
        //------------------------------------------------------

        m_x += AnimationStep;

        if (m_x >= m_targetX)
        {
            m_x = m_targetX;
            m_animating = false;
        }
    }
    //----------------------------------------------------------

    void MenuRenderer::drawCharClipped(
        Canvas &canvas,
        char c,
        int16_t x,
        uint8_t y)
    {
        const uint8_t width =
            Font::width();

        const uint8_t height =
            Font::height();

        const uint8_t *glyph =
            Font::glyph(c);

        if (glyph == nullptr)
            return;

        for (uint8_t col = 0; col < width; col++)
        {
            int16_t screenX =
                x + col;

            //--------------------------------------------------
            // Clipping horizontal
            //--------------------------------------------------

            if (screenX < 0)
                continue;

            if (screenX >= canvas.width())
                continue;

            uint8_t data =
                pgm_read_byte(
                    glyph + col);

            //--------------------------------------------------
            // Desenha coluna
            //--------------------------------------------------

            for (uint8_t row = 0;
                 row < height;
                 row++)
            {
                if (data & (1 << row))
                {
                    canvas.setPixel(
                        screenX,
                        y + row,
                        true);
                }
            }
        }
    }

    //----------------------------------------------------------

    void MenuRenderer::drawTextClipped(
        Canvas &canvas,
        const char *text,
        int16_t x,
        uint8_t y)
    {
        if (text == nullptr)
            return;

        const uint8_t step =
            Font::width() +
            Font::spacing();

        while (*text)
        {
            drawCharClipped(
                canvas,
                *text,
                x,
                y);

            x += step;

            text++;

            //--------------------------------------------------
            // Saiu completamente pela direita
            //--------------------------------------------------

            if (x >= canvas.width())
                break;
        }
    }

    //----------------------------------------------------------

    void MenuRenderer::draw(
        Canvas &canvas,
        const char *text)
    {
        if (text == nullptr)
            return;

        //------------------------------------------------------
        // Atualiza a animação
        //------------------------------------------------------

        update();

        //------------------------------------------------------
        // Animação em andamento
        //------------------------------------------------------

        if (m_animating)
        {
            drawTextClipped(
                canvas,
                text,
                m_x,
                0);

            return;
        }

        //------------------------------------------------------
        // Posição final centralizada
        //------------------------------------------------------

        int16_t x =
            centeredX(text);

        drawTextClipped(
            canvas,
            text,
            x,
            0);
    }
    //----------------------------------------------------------

    void MenuRenderer::startScrolling(
        const char *text)
    {
        m_scrollText = text;
        m_scrollX = Canvas::Width;
        m_lastScrollStep = millis();
    }

    //----------------------------------------------------------

    bool MenuRenderer::drawScrolling(
        Canvas &canvas,
        const char *text)
    {
        if (text == nullptr)
            return false;

        const uint32_t now = millis();

        if (m_scrollText != text)
        {
            m_scrollText = text;
            m_scrollX = canvas.width();
            m_lastScrollStep = now;
        }
        else if ((now - m_lastScrollStep) >= ScrollInterval)
        {
            m_lastScrollStep = now;
            --m_scrollX;
        }

        const int16_t textWidth = Graphics::textWidth(text);

        if (m_scrollX < -(textWidth + ScrollGap))
        {
            m_scrollX = canvas.width();
            drawTextClipped(canvas, text, m_scrollX, 0);
            return true;
        }

        drawTextClipped(canvas, text, m_scrollX, 0);

        return false;
    }

    //----------------------------------------------------------

    void MenuRenderer::drawEditing(
        Canvas &canvas,
        const char *label,
        int16_t value)
    {
        if (label == nullptr)
            return;

        //------------------------------------------------------
        // FONTE: mostra o nome da fonte selecionada.
        //------------------------------------------------------
        if (strcmp(label, "FONTE") == 0)
        {
            const char *name = "NORMAL";

            if (value == 1)
                name = "DUPLA";
            else if (value == 2)
                name = "SEGUNDOS";

            const uint8_t charW = Font::smallWidth();
            const uint8_t spacing = Font::smallSpacing();
            const uint8_t h = Font::smallHeight();

            uint8_t len = 0;
            while (name[len] != '\0')
                ++len;

            uint8_t totalW = (len * charW) +
                             ((len > 0) ? ((len - 1) * spacing) : 0);

            int16_t x = (canvas.width() - totalW) / 2;
            uint8_t y = (canvas.height() - h) / 2;

            for (uint8_t i = 0; i < len; ++i)
            {
                const uint8_t *glyph = Font::smallGlyph(name[i]);

                if (glyph != nullptr)
                {
                    for (uint8_t col = 0; col < charW; ++col)
                    {
                        uint8_t data = pgm_read_byte(glyph + col);

                        for (uint8_t row = 0; row < h; ++row)
                        {
                            if (data & (1 << row))
                                canvas.setPixel(x + col, y + row, true);
                        }
                    }
                }

                x += charW + spacing;
            }

            return;
        }

        //------------------------------------------------------
        // Converte o valor para texto
        //------------------------------------------------------

        char valueText[8];

        snprintf(
            valueText,
            sizeof(valueText),
            "%d",
            value);

        //------------------------------------------------------
        // Dimensões da fonte 5x3
        //------------------------------------------------------

        const uint8_t charWidth =
            Font::smallWidth();

        const uint8_t spacing =
            Font::smallSpacing();

        //------------------------------------------------------
        // Calcula largura do label
        //------------------------------------------------------

        uint8_t labelLength = 0;

        while (label[labelLength] != '\0')
            labelLength++;

        uint8_t valueLength = 0;

        while (valueText[valueLength] != '\0')
            valueLength++;

        //------------------------------------------------------
        // Largura total
        //------------------------------------------------------

        const uint8_t labelWidth =
            (labelLength * charWidth) +
            ((labelLength > 0)
                 ? ((labelLength - 1) * spacing)
                 : 0);

        const uint8_t valueWidth =
            (valueLength * charWidth) +
            ((valueLength > 0)
                 ? ((valueLength - 1) * spacing)
                 : 0);

        //------------------------------------------------------
        // Espaço entre nome e valor
        //------------------------------------------------------

        constexpr uint8_t valueGap = 2;

        const uint8_t totalWidth =
            labelWidth +
            valueGap +
            valueWidth;

        //------------------------------------------------------
        // Centraliza o conjunto inteiro
        //------------------------------------------------------

        int16_t x =
            (canvas.width() - totalWidth) / 2;

        //------------------------------------------------------
        // Centralização vertical da fonte 5x3
        //------------------------------------------------------

        const uint8_t y =
            (canvas.height() -
             Font::smallHeight()) /
            2;

        //------------------------------------------------------
        // Desenha LABEL
        //------------------------------------------------------

        const uint8_t *glyph;

        for (uint8_t i = 0;
             i < labelLength;
             i++)
        {
            glyph =
                Font::smallGlyph(label[i]);

            if (glyph != nullptr)
            {
                for (uint8_t col = 0;
                     col < charWidth;
                     col++)
                {
                    uint8_t data =
                        pgm_read_byte(
                            glyph + col);

                    for (uint8_t row = 0;
                         row < Font::smallHeight();
                         row++)
                    {
                        if (data & (1 << row))
                        {
                            canvas.setPixel(
                                x + col,
                                y + row,
                                true);
                        }
                    }
                }
            }

            x += charWidth + spacing;
        }

        //------------------------------------------------------
        // Espaço entre LABEL e VALOR
        //------------------------------------------------------

        x += valueGap;

        //------------------------------------------------------
        // Desenha VALUE
        //------------------------------------------------------

        for (uint8_t i = 0;
             i < valueLength;
             i++)
        {
            glyph =
                Font::smallGlyph(valueText[i]);

            if (glyph != nullptr)
            {
                for (uint8_t col = 0;
                     col < charWidth;
                     col++)
                {
                    uint8_t data =
                        pgm_read_byte(
                            glyph + col);

                    for (uint8_t row = 0;
                         row < Font::smallHeight();
                         row++)
                    {
                        if (data & (1 << row))
                        {
                            canvas.setPixel(
                                x + col,
                                y + row,
                                true);
                        }
                    }
                }
            }

            x += charWidth + spacing;
        }
    }
    //==========================================================
    // Navegação
    //==========================================================

    void nextItem()
    {
        if (itemCount == 0)
            return;

        currentIndex++;

        if (currentIndex >= itemCount)
            currentIndex = 0;

        renderer.startNext();
    }
    //----------------------------------------------------------
    void previousItem()
    {
        if (itemCount == 0)
            return;

        if (currentIndex == 0)
            currentIndex =
                itemCount - 1;
        else
            currentIndex--;

        renderer.startPrevious();
    }

    //==========================================================
    // Abrir Menu
    //==========================================================

    void openMenu()
    {
        if (itemCount == 0)
            return;

        currentState =
            MenuState::Browsing;

        currentIndex = 0;

        okLongPressActive = false;
        okRepeatCount = 0;

        renderer.reset();

        createResult(
            MenuResultType::Open,
            MenuItemId::None,
            0);
    }

    //==========================================================
    // OK mantido
    //==========================================================

    void processOkLongPress(
        const ButtonEvent &event)
    {
        //------------------------------------------------------
        // LongPress do OK
        //
        // 1000 ms = abrir Menu
        //------------------------------------------------------

        if (event.type ==
            ButtonEventType::LongPress)
        {
            openMenu();

            return;
        }
    }

    //==========================================================
    // Navegação
    //==========================================================

    void processBrowsing(
        const ButtonEvent &event)
    {
        if (aboutActive)
            return;

        //------------------------------------------------------
        // +
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Plus)
        {
            if (event.type ==
                    ButtonEventType::Click ||
                event.type ==
                    ButtonEventType::Repeat)
            {
                nextItem();
            }

            return;
        }

        //------------------------------------------------------
        // -
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Minus)
        {
            if (event.type ==
                    ButtonEventType::Click ||
                event.type ==
                    ButtonEventType::Repeat)
            {
                previousItem();
            }

            return;
        }
        //------------------------------------------------------
        // HOME
        // Sai do menu
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Home)
        {
            if (event.type ==
                ButtonEventType::Click)
            {
                Menu::close();
            }

            return;
        }
        //------------------------------------------------------
        // OK
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Ok)
        {
            if (event.type !=
                ButtonEventType::Click)
            {
                return;
            }

            if (itemCount == 0)
                return;

            //------------------------------------------------------
            // Item HORA / DATA
            //
            // Ambos entram no mesmo ajuste completo de
            // DATA + HORA.
            //------------------------------------------------------

            if (items[currentIndex].id ==
                    MenuItemId::Time ||
                items[currentIndex].id ==
                    MenuItemId::Date)
            {
                if (items[currentIndex].id ==
                    MenuItemId::Date)
                {
                    RTCAdjust::start(
                        RTCAdjust::Mode::Date);
                }
                else
                {
                    RTCAdjust::start(
                        RTCAdjust::Mode::Time);
                }

                rtcAdjustActive = true;

                createResult(
                    MenuResultType::Selected,
                    items[currentIndex].id,
                    0);

                return;
            }
            //------------------------------------------------------
            // Item ALARME
            //------------------------------------------------------

            if (items[currentIndex].id ==
                MenuItemId::Alarm)
            {
                Alarm::start();

                alarmActive = true;

                createResult(
                    MenuResultType::Selected,
                    MenuItemId::Alarm,
                    0);

                return;
            }

            //------------------------------------------------------
            // Item SOBRE
            //------------------------------------------------------

            if (items[currentIndex].id ==
                MenuItemId::About)
            {
                aboutActive = true;
                aboutScrollCount = 0;

                renderer.startScrolling(
                    "ALBERTO NEVES AGOSTO 2026");

                return;
            }

            //------------------------------------------------------
            // Item editável
            //------------------------------------------------------

            if (items[currentIndex].edit.editable)
            {
                currentValue =
                    items[currentIndex].edit.minimum;

                currentState =
                    MenuState::Editing;

                createResult(
                    MenuResultType::Selected,
                    items[currentIndex].id,
                    currentValue);

                return;
            }

            //------------------------------------------------------
            // Item somente seleção
            //------------------------------------------------------

            createResult(
                MenuResultType::Selected,
                items[currentIndex].id,
                currentValue);
        }
    }

    //==========================================================
    // Edição
    //==========================================================

    void processEditing(
        const ButtonEvent &event)
    {
        //------------------------------------------------------
        // +
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Plus)
        {
            if (event.type ==
                    ButtonEventType::Click ||
                event.type ==
                    ButtonEventType::Repeat)
            {
                increaseValue();

                createResult(
                    MenuResultType::Changed,
                    items[currentIndex].id,
                    currentValue);
            }

            return;
        }

        //------------------------------------------------------
        // -
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Minus)
        {
            if (event.type ==
                    ButtonEventType::Click ||
                event.type ==
                    ButtonEventType::Repeat)
            {
                decreaseValue();

                createResult(
                    MenuResultType::Changed,
                    items[currentIndex].id,
                    currentValue);
            }

            return;
        }
        //------------------------------------------------------
        // HOME
        // Sai do menu
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Home)
        {
            if (event.type ==
                ButtonEventType::Click)
            {
                Menu::close();
            }

            return;
        }
        //------------------------------------------------------
        // OK
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Ok)
        {
            if (event.type ==
                ButtonEventType::Click)
            {
                createResult(
                    MenuResultType::Confirmed,
                    items[currentIndex].id,
                    currentValue);

                currentState =
                    MenuState::Browsing;
            }
        }
    }

} // namespace

//==========================================================
// API pública
//==========================================================

namespace Menu
{

    //======================================================
    // Inicialização
    //======================================================

    bool begin()
    {
        itemCount = 0;

        currentIndex = 0;

        currentState =
            MenuState::Closed;

        currentValue = 0;

        okLongPressActive = false;

        okRepeatCount = 0;

        clearResult();

        renderer.reset();

        Alarm::begin();

        alarmActive = false;

        aboutActive = false;
        aboutScrollCount = 0;

        RTCAdjust::begin();

        rtcAdjustActive = false;

        return true;
    }

    //======================================================
    // Limpa itens
    //======================================================

    void clear()
    {
        itemCount = 0;

        currentIndex = 0;

        currentState =
            MenuState::Closed;

        currentValue = 0;

        clearResult();

        renderer.reset();

        Alarm::begin();

        alarmActive = false;

        aboutActive = false;
        aboutScrollCount = 0;
    }

    //======================================================
    // Adiciona item
    //======================================================
    // ver nova
    bool addItem(
        MenuItemId id,
        const char *name,
        const MenuEditConfig &edit)
    {
        if (itemCount >= MaxItems)
            return false;

        if (name == nullptr)
            return false;

        items[itemCount].id = id;

        items[itemCount].name = name;

        items[itemCount].edit = edit;

        itemCount++;

        return true;
    }
    // ver antiga
    bool addItem(
        MenuItemId id,
        const char *name)
    {
        MenuEditConfig edit =
            {
                false,
                0,
                0,
                1};

        return addItem(
            id,
            name,
            edit);
    }
    //======================================================
    // Atualização
    //======================================================

    void update(
        const ButtonEvent &event)
    {
        //--------------------------------------------------
        // Menu fechado
        //--------------------------------------------------
        //--------------------------------------------------
        // Alarme ativo
        //--------------------------------------------------

        if (alarmActive)
        {
            Alarm::update(event);

            if (Alarm::state() ==
                Alarm::State::Inactive)
            {
                alarmActive = false;

                currentState =
                    MenuState::Browsing;

                createResult(
                    MenuResultType::Confirmed,
                    MenuItemId::Alarm,
                    (Alarm::hour() * 100) +
                        Alarm::minute());
            }

            return;
        }

        //--------------------------------------------------
        // Ajuste RTC ativo
        //--------------------------------------------------

        if (rtcAdjustActive)
        {
            RTCAdjust::update(event);

            RTCAdjust::Result rtcResult;

            if (RTCAdjust::readResult(rtcResult))
            {
                //--------------------------------------------------
                // CANCELADO
                //--------------------------------------------------

                if (rtcResult ==
                    RTCAdjust::Result::Cancelled)
                {
                    rtcAdjustActive = false;

                    currentState =
                        MenuState::Browsing;

                    return;
                }

                //--------------------------------------------------
                // CONFIRMADO
                //--------------------------------------------------

                if (rtcResult ==
                    RTCAdjust::Result::Confirmed)
                {
                    rtcAdjustActive = false;

                    currentState =
                        MenuState::Browsing;

                    createResult(
                        MenuResultType::Confirmed,
                        MenuItemId::Time,
                        0);

                    return;
                }
            }

            return;
        }
        //--------------------------------------------------
        if (currentState ==
            MenuState::Closed)
        {
            if (event.button ==
                ButtonId::Ok)
            {
                processOkLongPress(event);
            }

            return;
        }

        //--------------------------------------------------
        // Menu navegando
        //--------------------------------------------------

        if (currentState ==
            MenuState::Browsing)
        {
            processBrowsing(event);

            return;
        }

        //--------------------------------------------------
        // Menu em edição
        //--------------------------------------------------

        if (currentState ==
            MenuState::Editing)
        {
            processEditing(event);

            return;
        }
    }

    //======================================================
    // Renderização
    //======================================================

    void draw(
        Canvas &canvas)
    {
        if (currentState ==
            MenuState::Closed)
        {
            return;
        }

        if (itemCount == 0)
            return;

        canvas.clear();
        //--------------------------------------------------
        // Alarme
        //--------------------------------------------------

        if (alarmActive)
        {
            Alarm::draw(canvas);
            return;
        }

        //--------------------------------------------------
        // Ajuste RTC
        //--------------------------------------------------

        if (rtcAdjustActive)
        {
            RTCAdjust::draw(canvas);
            return;
        }
        //--------------------------------------------------
        // Edição
        //--------------------------------------------------

        if (currentState ==
            MenuState::Editing)
        {
            renderer.drawEditing(
                canvas,
                items[currentIndex].name,
                currentValue);

            return;
        }

        //--------------------------------------------------
        // Navegação
        //--------------------------------------------------

        if (aboutActive)
        {
            if (renderer.drawScrolling(
                canvas,
                "ALBERTO NEVES AGOSTO 2026"))
            {
                ++aboutScrollCount;

                if (aboutScrollCount >= 2)
                {
                    Menu::close();
                    return;
                }
            }

            return;
        }

        renderer.draw(
            canvas,
            items[currentIndex].name);
    }

    //======================================================
    // Resultado
    //======================================================

    bool readResult(
        MenuResult &result)
    {
        if (pendingResult.type ==
            MenuResultType::None)
        {
            return false;
        }

        result =
            pendingResult;

        clearResult();

        return true;
    }

    //======================================================
    // Estado
    //======================================================

    MenuState state()
    {
        return currentState;
    }

    //======================================================
    // Item selecionado
    //======================================================

    MenuItemId selected()
    {
        if (itemCount == 0)
            return MenuItemId::None;

        return items[currentIndex].id;
    }

    //======================================================
    // Inicia edição
    //======================================================

    void beginEdit(
        int16_t value)
    {
        if (itemCount == 0)
            return;

        currentValue = value;

        currentState =
            MenuState::Editing;
    }

    //======================================================
    // Define valor
    //======================================================

    void setValue(
        int16_t value)
    {
        currentValue = value;
    }

    //======================================================
    // Retorna valor
    //======================================================

    int16_t value()
    {
        return currentValue;
    }

    //======================================================
    // Fecha Menu
    //======================================================

    void close()
    {
        currentState =
            MenuState::Closed;

        okLongPressActive = false;

        okRepeatCount = 0;

        aboutActive = false;
        aboutScrollCount = 0;

        renderer.reset();

        clearResult();
    }

} // namespace Menu
