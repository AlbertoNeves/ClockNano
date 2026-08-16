#pragma once

#include <Arduino.h>

#include <Buttons.h>
#include <Canvas.h>

namespace Melodias
{
    enum class Id : uint8_t
    {
        None = 0,
        Simpsons,
        Indiana,
        TakeOnMe,
        Entertainer,
        Muppets,
        XFiles,
        LooneyTunes,
        Fox20thCentury,
        JamesBond,
        Mash,
        StarWars,
        GoodBadUgly,
        TopGun,
        ATeam,
        Flinstones,
        Jeopardy,
        Gadget,
        Smurfs,
        MahnaMahna,
        LeisureSuit,
        MissionImpossible,
        Count
    };

    // Reproduz uma melodia uma única vez, sem bloquear o relógio.
    void play(Id id);
    void stop();
    void service();

    // Reproduz a melodia configurada para: 0=ÚNICO, 1=DIÁRIO, 2=PERSONALIZADO.
    void playConfigured(uint8_t category);

    // Tela de configuração acessada pelo item MELODIAS do menu principal.
    void start();
    void cancel();
    void update(const ButtonEvent &event);
    void draw(Canvas &canvas);
    bool active();
    bool isPlaying();

    // Indica que a melodia em reprodução foi disparada por um alarme.
    bool isAlarmPlaying();
}
