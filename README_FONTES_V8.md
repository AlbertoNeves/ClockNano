# ClockNano — V8 Seconds Odometer

Correção da animação vertical dos segundos.

- Remove a linha horizontal preenchida da V7.
- Mantém 1 linha de pixels apagados entre o dígito que sai e o dígito que entra.
- A separação permanece durante toda a animação.
- O dígito das dezenas continua imóvel quando seu valor não muda.
- A transição usa deslocamento máximo de 6 pixels (5 pixels da fonte + 1 pixel de separação).
- NORMAL e DUPLA permanecem inalteradas.

## Configuração

- Arduino Nano ATmega328
- 4 módulos MAX7219 8x8 (32x8)
- MD_MAX72XX 3.5.1
- RTClib 2.1.1
- Plataforma definida em `platformio.ini`.
