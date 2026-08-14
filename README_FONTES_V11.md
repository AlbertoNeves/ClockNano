# ClockNano — V11

## Alterações

- **DUPLA**: mantém `dig6x8` exatamente como aprovado na V10.
- **SEGUNDOS**:
  - `HH:MM` passa a usar `dig5x8rn` (5x8), baseada no `fonts.h` fornecido.
  - segundos continuam usando `Font5x3` (3x5).
  - animação **Seconds Odometer** foi mantida.
- **NORMAL**: deixa de usar `Graphics::drawStringCentered()` na tela principal.
  - `HH:MM` passa a usar posições fixas.
  - mantém 1 pixel de espaçamento entre os elementos.

## Observação

A centralização automática continua disponível no `Graphics` para textos e telas que precisem dela; ela não é mais usada para o relógio principal.
