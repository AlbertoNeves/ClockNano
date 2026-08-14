# ClockNano — V7 — Linha de separação no Seconds Odometer

Evolução da V6, mantendo os quatro módulos MAX7219 8x8 (32x8).

## Alteração

Durante a rolagem vertical dos segundos em fonte 5x3, foi acrescentada uma linha horizontal de 7 pixels que acompanha a fronteira entre o dígito antigo, que está saindo pelo topo, e o novo dígito, que entra pela parte inferior.

A linha só aparece durante a transição e acompanha o movimento da janela do odômetro.

## Mantido

- NORMAL e DUPLA inalteradas.
- Fonte dos segundos continua Font5x3 (3x5).
- 37 -> 38: somente unidades rolam.
- 39 -> 40: dezenas e unidades rolam.
- 59 -> 00: os dois rolam.
- Animação não bloqueante.
- Duração inicial de 200 ms.
