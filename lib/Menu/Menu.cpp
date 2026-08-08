#include "Menu.h"

#include <Arduino.h>
#include <Graphics.h>
#include <Font.h>

#include <avr/pgmspace.h>

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

    constexpr uint16_t AnimationInterval = 25;

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

        uint32_t m_lastStep = 0;
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

        m_lastStep = millis();
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
        m_directionNext = true;

        m_animating = true;

        m_x = 32;

        m_lastStep = millis();
    }

    //----------------------------------------------------------

    void MenuRenderer::startPrevious()
    {
        m_directionNext = false;

        m_animating = true;

        m_x = -32;

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
        // Próximo item:
        //
        // entra pela direita
        //------------------------------------------------------

        if (m_directionNext)
        {
            m_x -= AnimationStep;

            if (m_x <= 0)
            {
                m_animating = false;
            }

            return;
        }

        //------------------------------------------------------
        // Item anterior:
        //
        // entra pela esquerda
        //------------------------------------------------------

        m_x += AnimationStep;

        if (m_x >= 0)
        {
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
        // OK
        //------------------------------------------------------

        if (event.button ==
            ButtonId::Ok)
        {
            if (event.type ==
                ButtonEventType::Click)
            {
                if (itemCount == 0)
                    return;

                createResult(
                    MenuResultType::Selected,
                    items[currentIndex].id,
                    currentValue);
            }
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
                currentValue++;

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
                currentValue--;

                createResult(
                    MenuResultType::Changed,
                    items[currentIndex].id,
                    currentValue);
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
    }

    //======================================================
    // Adiciona item
    //======================================================

    bool addItem(
        MenuItemId id,
        const char *name)
    {
        if (itemCount >= MaxItems)
            return false;

        if (name == nullptr)
            return false;

        items[itemCount].id = id;

        items[itemCount].name = name;

        itemCount++;

        return true;
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

        renderer.reset();

        clearResult();
    }

} // namespace Menu