#include "Buttons.h"

#include <Arduino.h>
#include <Config.h>

namespace
{

    //==========================================================
    // Configuração do módulo
    //==========================================================

    namespace Settings
    {
        constexpr uint16_t DebounceTime = 15;
        constexpr uint16_t LongPressTime = 1000;
        constexpr uint16_t RepeatDelay = 250;
        constexpr uint16_t RepeatPeriod = 120;

        constexpr uint8_t EventQueueSize = 8;
    }

    //==========================================================
    // Eventos físicos
    //==========================================================

    enum class Edge : uint8_t
    {
        None,
        Press,
        Release
    };

    //==========================================================
    // Máquina de estados
    //==========================================================

    enum class ButtonState : uint8_t
    {
        Idle,
        Pressed,
        LongPressed,
        Repeating
    };

    //==========================================================
    // Contexto da atualização
    //==========================================================

    struct UpdateContext
    {
        uint32_t now;
    };

    //==========================================================
    // FIFO de eventos
    //==========================================================

    class EventQueue
    {
    public:
        void clear();

        bool push(
            const ButtonEvent &event);

        bool pop(
            ButtonEvent &event);

        bool empty() const;

    private:
        ButtonEvent m_queue[Settings::EventQueueSize];

        uint8_t m_head = 0;

        uint8_t m_tail = 0;
    };

    //==========================================================
    // Classe Button
    //==========================================================

    class Button
    {
    public:
        void begin(
            uint8_t pin,
            ButtonId id);

        bool update(
            const UpdateContext &ctx,
            ButtonEvent &event);

    private:
        //------------------------------------------------------
        // Hardware
        //------------------------------------------------------

        bool readHardware();

        Edge detectEdge(
            bool sample,
            uint32_t now);

        /*------------------------------------------------------
        // Máquina de estados
        //------------------------------------------------------
        Resultado desta FSM
        Ação	Evento
        Pressionar	(nenhum)
        Soltar antes de 700 ms	Click
        Manter pressionado 700 ms	LongPress
        Permanecer pressionado +250 ms	Repeat
        Permanecer pressionado	Repeat a cada 120 ms
        Soltar após LongPress/Repeat	Release
        */
        //==========================================================
        bool stateIdle(
            Edge edge,
            const UpdateContext &ctx,
            ButtonEvent &)
        {
            if (edge != Edge::Press)
                return false;

            setState(ButtonState::Pressed);

            startPressTimer(ctx.now);

            return false;
        }
        //==========================================================
        bool statePressed(
            Edge edge,
            const UpdateContext &ctx,
            ButtonEvent &event)
        {
            //-----------------------------------------
            // Clique
            //-----------------------------------------

            if (edge == Edge::Release)
            {
                Serial.print("RELEASE -> CLICK  ");
                Serial.println(ctx.now - m_pressTime);
                
                createEvent(
                    event,
                    ButtonEventType::Click,
                    ctx.now);

                setState(ButtonState::Idle);

                return true;
            }

            //-----------------------------------------
            // Long Press
            //-----------------------------------------

            if ((ctx.now - m_pressTime) >=
                Settings::LongPressTime)
            {
                createEvent(
                    event,
                    ButtonEventType::LongPress,
                    ctx.now);

                setState(ButtonState::LongPressed);

                startRepeatTimer(ctx.now);

                return true;
            }

            return false;
        }
        //==========================================================
        bool stateLongPressed(
            Edge edge,
            const UpdateContext &ctx,
            ButtonEvent &event)
        {
            //-----------------------------------------
            // Soltou
            //-----------------------------------------

            if (edge == Edge::Release)
            {
                createEvent(
                    event,
                    ButtonEventType::Release,
                    ctx.now);

                setState(ButtonState::Idle);

                return true;
            }

            //-----------------------------------------
            // Primeiro Repeat
            //-----------------------------------------

            if ((ctx.now - m_repeatTime) >=
                Settings::RepeatDelay)
            {
                createEvent(
                    event,
                    ButtonEventType::Repeat,
                    ctx.now);

                setState(ButtonState::Repeating);

                startRepeatTimer(ctx.now);

                return true;
            }

            return false;
        }

        //==========================================================

        bool stateRepeating(
            Edge edge,
            const UpdateContext &ctx,
            ButtonEvent &event)
        {
            //-----------------------------------------
            // Soltou
            //-----------------------------------------

            if (edge == Edge::Release)
            {
                createEvent(
                    event,
                    ButtonEventType::Release,
                    ctx.now);

                setState(ButtonState::Idle);

                return true;
            }

            //-----------------------------------------
            // Repeat
            //-----------------------------------------

            if ((ctx.now - m_repeatTime) >=
                Settings::RepeatPeriod)
            {
                createEvent(
                    event,
                    ButtonEventType::Repeat,
                    ctx.now);

                startRepeatTimer(ctx.now);

                return true;
            }

            return false;
        }

        //------------------------------------------------------
        // Auxiliares
        //------------------------------------------------------

        void setState(
            ButtonState state);

        void startPressTimer(
            uint32_t now);

        void startRepeatTimer(
            uint32_t now);

        void createEvent(
            ButtonEvent &event,
            ButtonEventType type,
            uint32_t now);

    private:
        //------------------------------------------------------
        // Hardware
        //------------------------------------------------------

        uint8_t m_pin = 0;

        ButtonId m_id = ButtonId::None;

        //------------------------------------------------------
        // Estado
        //------------------------------------------------------

        ButtonState m_state = ButtonState::Idle;

        //------------------------------------------------------
        // Debounce
        //------------------------------------------------------

        bool m_lastSample = HIGH;

        bool m_stableState = HIGH;

        uint32_t m_lastTransition = 0;

        //------------------------------------------------------
        // Timers
        //------------------------------------------------------

        uint32_t m_pressTime = 0;

        uint32_t m_repeatTime = 0;
    };

    //==========================================================
    // Objetos do módulo
    //==========================================================

    EventQueue g_queue;

    Button g_buttons[3];
    //==========================================================

    void EventQueue::clear()
    {
        m_head = 0;
        m_tail = 0;
    }

    //==========================================================

    bool EventQueue::empty() const
    {
        return (m_head == m_tail);
    }

    //==========================================================

    bool EventQueue::push(
        const ButtonEvent &event)
    {
        uint8_t next =
            (m_head + 1) % Settings::EventQueueSize;

        if (next == m_tail)
            return false;

        m_queue[m_head] = event;

        m_head = next;

        return true;
    }

    //==========================================================

    bool EventQueue::pop(
        ButtonEvent &event)
    {
        if (empty())
            return false;

        event = m_queue[m_tail];

        m_tail =
            (m_tail + 1) %
            Settings::EventQueueSize;

        return true;
    }
    //==========================================================

    void Button::begin(
        uint8_t pin,
        ButtonId id)
    {
        m_pin = pin;

        m_id = id;

        pinMode(
            m_pin,
            INPUT_PULLUP);

        m_lastSample =
            digitalRead(m_pin);

        m_stableState =
            m_lastSample;

        m_lastTransition =
            millis();

        m_state =
            ButtonState::Idle;

        m_pressTime = 0;

        m_repeatTime = 0;
    }
    //==========================================================

    bool Button::readHardware()
    {
        return digitalRead(m_pin);
    }
    //==========================================================
    //==========================================================

    Edge Button::detectEdge(
        bool sample,
        uint32_t now)
    {
        if (sample != m_lastSample)
        {
            m_lastSample = sample;

            m_lastTransition = now;

            return Edge::None;
        }

        if ((now - m_lastTransition) <
            Settings::DebounceTime)
        {
            return Edge::None;
        }

        if (sample == m_stableState)
            return Edge::None;

        m_stableState = sample;

        return sample == LOW
                   ? Edge::Press
                   : Edge::Release;
    }
    //==========================================================
    void Button::createEvent(
        ButtonEvent &event,
        ButtonEventType type,
        uint32_t now)
    {
        event.button = m_id;

        event.type = type;

        event.timestamp = now;
    }
    //===========================================================
    void Button::setState(
        ButtonState state)
    {
        m_state = state;
    }
    //==========================================================

    void Button::startPressTimer(
        uint32_t now)
    {
        m_pressTime = now;
    }
    //==========================================================

    void Button::startRepeatTimer(
        uint32_t now)
    {
        m_repeatTime = now;
    }
    //==========================================================

    bool Button::update(
        const UpdateContext &ctx,
        ButtonEvent &event)
    {
        bool sample =
            readHardware();

        Edge edge =
            detectEdge(
                sample,
                ctx.now);

        switch (m_state)
        {
        case ButtonState::Idle:

            return stateIdle(
                edge,
                ctx,
                event);

        case ButtonState::Pressed:

            return statePressed(
                edge,
                ctx,
                event);

        case ButtonState::LongPressed:

            return stateLongPressed(
                edge,
                ctx,
                event);

        case ButtonState::Repeating:

            return stateRepeating(
                edge,
                ctx,
                event);
        }

        return false;
    } //==========================================================
} // namespace

//==========================================================

namespace Buttons
{

    bool begin()
    {
        g_queue.clear();

        g_buttons[0].begin(
            Config::BtnMinus,
            ButtonId::Minus);

        g_buttons[1].begin(
            Config::BtnOk,
            ButtonId::Ok);

        g_buttons[2].begin(
            Config::BtnPlus,
            ButtonId::Plus);

        return true;
    }

    //==========================================================

    void update()
    {
        UpdateContext ctx;

        ctx.now = millis();

        ButtonEvent event;

        for (Button &button : g_buttons)
        {
            if (button.update(
                    ctx,
                    event))
            {
                g_queue.push(event);
            }
        }
    }

    //==========================================================

    bool read(
        ButtonEvent &event)
    {
        return g_queue.pop(event);
    }

    //==========================================================

    void clear()
    {
        g_queue.clear();
    }

} // namespace Buttons
