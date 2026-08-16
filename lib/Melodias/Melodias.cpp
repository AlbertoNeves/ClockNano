#include "Melodias.h"
#include "MelodyData.h"

#include <Config.h>
#include <ConfigEEPROM.h>
#include <Font.h>


namespace
{
    // Melodias RTTTL ativas no sketch fornecido pelo usuário.
    const char songFlinstones[] PROGMEM =
        "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";

    const char songMissionImpossible[] PROGMEM =
        "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";

    const char songEntertainer[] PROGMEM =
        "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";

    const char songMahnaMahna[] PROGMEM =
        "MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";

    const char *const songs[] PROGMEM =
    {
        nullptr,
        songFlinstones,
        songMissionImpossible,
        songEntertainer,
        songMahnaMahna
    };

    const char *const melodyNames[] =
    {
        "NENHUMA", "FLINSTONES", "MISSAO", "ENTERTAINER", "MAHNA"
    };

    const char *const categoryNames[] =
    {
        "UNICO", "DIARIO", "PERSONALIZADO"
    };

    const uint16_t noteFrequencies[] PROGMEM =
    {
        262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
        523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
        1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
        2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
    };

    const char *song = nullptr;
    uint16_t songPosition = 0;
    uint32_t wholeNote = 0;
    uint8_t defaultDuration = 4;
    uint8_t defaultOctave = 6;
    uint32_t noteEndsAt = 0;
    bool playing = false;
    bool alarmPlayback = false;

    bool menuActive = false;
    bool selectingMelody = false;
    uint8_t selectedCategory = 0;
    Melodias::Id selectedMelody = Melodias::Id::None;

    // Garante um frame limpo antes de exibir um novo nome de melodia.
    bool clearBeforeMelodyName = false;

    // Rolagem do nome da melodia selecionada.
    int16_t melodyScrollX = 0;
    uint32_t melodyScrollAt = 0;
    constexpr uint16_t MelodyScrollInterval = 100;
    constexpr uint8_t MelodyScrollGap = 4;

    char readSong()
    {
        return static_cast<char>(pgm_read_byte(song + songPosition));
    }

    uint16_t readNumber()
    {
        uint16_t value = 0;
        while (readSong() >= '0' && readSong() <= '9')
        {
            value = (value * 10U) + (readSong() - '0');
            ++songPosition;
        }
        return value;
    }

    void startNextNote()
    {
        const char current = readSong();
        if (current == '\0')
        {
            Melodias::stop();
            return;
        }

        uint16_t divisor = readNumber();
        if (divisor == 0)
            divisor = defaultDuration;

        uint32_t duration = wholeNote / divisor;
        char noteChar = readSong();
        ++songPosition;

        int8_t note = -1;
        switch (noteChar)
        {
        case 'c': note = 0; break;
        case 'd': note = 2; break;
        case 'e': note = 4; break;
        case 'f': note = 5; break;
        case 'g': note = 7; break;
        case 'a': note = 9; break;
        case 'b': note = 11; break;
        default: break;
        }

        if (readSong() == '#')
        {
            ++note;
            ++songPosition;
        }

        if (readSong() == '.')
        {
            duration += duration / 2U;
            ++songPosition;
        }

        uint8_t octave = defaultOctave;
        if (readSong() >= '0' && readSong() <= '9')
            octave = readSong() - '0', ++songPosition;

        if (readSong() == ',')
            ++songPosition;

        if (note >= 0 && octave >= 4 && octave <= 7)
        {
            const uint8_t index = (octave - 4U) * 12U + note;
            if (index < (sizeof(noteFrequencies) / sizeof(noteFrequencies[0])))
                tone(Config::Buzzer, pgm_read_word(noteFrequencies + index));
        }
        else
        {
            noTone(Config::Buzzer);
        }

        noteEndsAt = millis() + duration;
    }

    uint8_t textLength(const char *text)
    {
        uint8_t length = 0;
        while (text[length] != '\0')
            ++length;
        return length;
    }

    uint16_t smallTextWidth(const char *text)
    {
        const uint8_t length = textLength(text);
        if (length == 0)
            return 0;

        return static_cast<uint16_t>(length) * Font::smallWidth() +
               static_cast<uint16_t>(length - 1U) * Font::smallSpacing();
    }

    void drawSmallText(Canvas &canvas, const char *text, int16_t x)
    {
        const uint8_t charW = Font::smallWidth();
        const uint8_t spacing = Font::smallSpacing();
        const uint8_t h = Font::smallHeight();
        const uint8_t len = textLength(text);
        const int16_t y = (canvas.height() - h) / 2;

        for (uint8_t i = 0; i < len; ++i)
        {
            const uint8_t *glyph = Font::smallGlyph(text[i]);

            if (glyph != nullptr)
            {
                for (uint8_t col = 0; col < charW; ++col)
                {
                    const int16_t screenX = x + col;
                    if (screenX < 0 || screenX >= canvas.width())
                        continue;

                    const uint8_t data = pgm_read_byte(glyph + col);
                    for (uint8_t row = 0; row < h; ++row)
                    {
                        if (data & (1 << row))
                            canvas.setPixel(screenX, y + row, true);
                    }
                }
            }

            x += charW + spacing;
        }
    }

    void resetMelodyScroll()
    {
        melodyScrollX = Canvas::Width / 2;
        melodyScrollAt = millis();
    }

    void drawCategory(Canvas &canvas, const char *text)
    {
        const int16_t width = smallTextWidth(text);
        const int16_t x = (canvas.width() - width) / 2;
        drawSmallText(canvas, text, x);
    }

    void drawSelectedMelody(Canvas &canvas, const char *text)
    {
        const uint16_t width = smallTextWidth(text);
        const uint32_t now = millis();

        if ((now - melodyScrollAt) >= MelodyScrollInterval)
        {
            melodyScrollAt = now;
            --melodyScrollX;

            if (melodyScrollX < -static_cast<int16_t>(width + MelodyScrollGap))
                melodyScrollX = canvas.width();
        }

        drawSmallText(canvas, text, melodyScrollX);
    }

}

namespace Melodias
{
    void play(Id id)
    {
        stop();
        if (id == Id::None || id >= Id::Count)
            return;

        song = reinterpret_cast<const char *>(pgm_read_ptr(MelodyData::songs + static_cast<uint8_t>(id)));
        songPosition = 0;

        while (readSong() != ':' && readSong() != '\0')
            ++songPosition;
        if (readSong() == '\0')
            return;
        ++songPosition;

        if (readSong() == 'd')
        {
            songPosition += 2;
            const uint16_t value = readNumber();
            if (value > 0)
                defaultDuration = value;
            if (readSong() == ',')
                ++songPosition;
        }
        if (readSong() == 'o')
        {
            songPosition += 2;
            const uint16_t value = readNumber();
            if (value >= 4 && value <= 7)
                defaultOctave = value;
            if (readSong() == ',')
                ++songPosition;
        }
        uint16_t bpm = 120;
        if (readSong() == 'b')
        {
            songPosition += 2;
            bpm = readNumber();
            if (readSong() == ':')
                ++songPosition;
        }
        wholeNote = (60000UL / bpm) * 4UL;
        playing = true;
        startNextNote();
    }

    void stop()
    {
        noTone(Config::Buzzer);
        playing = false;
        song = nullptr;
        alarmPlayback = false;
    }

    void service()
    {
        if (playing && static_cast<int32_t>(millis() - noteEndsAt) >= 0)
        {
            noTone(Config::Buzzer);
            startNextNote();
        }
    }

    void playConfigured(uint8_t category)
    {
        play(static_cast<Id>(ConfigEEPROM::loadMelody(category)));
        alarmPlayback = playing;
    }

    void start()
    {
        menuActive = true;
        selectingMelody = false;
        selectedCategory = 0;
        clearBeforeMelodyName = false;
        resetMelodyScroll();
    }

    void cancel()
    {
        stop();
        menuActive = false;
        selectingMelody = false;
    }

    void update(const ButtonEvent &event)
    {
        if (!menuActive || event.type != ButtonEventType::Click)
            return;

        if (event.button == ButtonId::Home)
        {
            if (selectingMelody)
            {
                stop();
                selectingMelody = false;
                clearBeforeMelodyName = true;
            }
            else
            {
                menuActive = false;
            }
            return;
        }

        if (!selectingMelody)
        {
            if (event.button == ButtonId::Plus)
                selectedCategory = (selectedCategory + 1U) % 3U;
            else if (event.button == ButtonId::Minus)
                selectedCategory = (selectedCategory + 2U) % 3U;
            else if (event.button == ButtonId::Ok)
            {
                selectingMelody = true;
                selectedMelody = static_cast<Id>(ConfigEEPROM::loadMelody(selectedCategory));
                clearBeforeMelodyName = true;
                resetMelodyScroll();
                play(selectedMelody);
            }
            return;
        }

        if (event.button == ButtonId::Plus || event.button == ButtonId::Minus)
        {
            uint8_t value = static_cast<uint8_t>(selectedMelody);
            const uint8_t count = static_cast<uint8_t>(Id::Count);
            value = event.button == ButtonId::Plus ? (value + 1U) % count : (value + count - 1U) % count;
            selectedMelody = static_cast<Id>(value);
            clearBeforeMelodyName = true;
            resetMelodyScroll();
            play(selectedMelody);
        }
        else if (event.button == ButtonId::Ok)
        {
            ConfigEEPROM::saveMelody(selectedCategory, static_cast<uint8_t>(selectedMelody));
            stop();
            selectingMelody = false;
            clearBeforeMelodyName = true;
        }
    }

    void draw(Canvas &canvas)
    {
        canvas.clear();

        // Ao trocar a melodia, deixa um frame totalmente apagado antes
        // de desenhar o novo nome. Isso evita qualquer resíduo visual
        // do nome anterior, principalmente quando ele era mais longo.
        if (clearBeforeMelodyName)
        {
            clearBeforeMelodyName = false;
            return;
        }

        if (selectingMelody)
            drawSelectedMelody(canvas, MelodyData::names[static_cast<uint8_t>(selectedMelody)]);
        else
            drawCategory(canvas, categoryNames[selectedCategory]);
    }

    bool active()
    {
        return menuActive;
    }

    bool isPlaying()
    {
        return playing;
    }

    bool isAlarmPlaying()
    {
        return playing && alarmPlayback;
    }
}
