# ClockNano — V6 — Seconds Odometer

## Modo SEGUNDOS

Evolução da V5 mantendo os quatro módulos MAX7219 8x8 (32x8).

### Animação dos segundos

- Fonte dos segundos: `Font5x3` (3x5 pixels).
- Os dois dígitos continuam lado a lado nas 7 colunas reservadas.
- O dígito antigo sai verticalmente pelo topo.
- O novo dígito entra verticalmente pela parte inferior.
- Duração inicial da transição: 200 ms.
- Movimento com curva `smoothstep` (acelera no meio e desacelera no final).
- Não usa `delay()`, mantendo a atualização não bloqueante.

### Comportamento de odômetro

- `37 -> 38`: somente o dígito das unidades rola.
- `38 -> 39`: somente as unidades rolam.
- `39 -> 40`: dezenas e unidades rolam simultaneamente.
- `59 -> 00`: os dois dígitos rolam simultaneamente.

### Layout

- HH:MM continua exatamente no layout compacto da V5.
- SS continua ocupando `3 + 1 + 3 = 7` colunas.
- NORMAL e DUPLA permanecem inalteradas.

### Hardware

Nenhuma alteração de hardware. Continua usando exatamente quatro módulos 8x8, totalizando 32x8 pixels.
