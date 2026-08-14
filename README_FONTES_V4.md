# ClockNano — Fontes V4

## Modo SEGUNDOS — layout compacto 32x8

Nesta versão o modo SEGUNDOS foi reorganizado para usar os quatro módulos 8x8 sem sobreposição.

### Layout

- Hora: `5 + 1 + 5` pixels
- `:` compacto: `3` colunas lógicas, sem espaçamento externo
- Minutos: `5 + 1 + 5` pixels
- Segundos: `3 + 1 + 3` pixels, lado a lado

Total: `25 + 7 = 32` pixels.

O espaçamento automático do `Graphics::drawString()` não é usado no modo SEGUNDOS. Isso permite retirar o espaço entre a unidade da hora e `:`, e entre `:` e a dezena dos minutos.

Os segundos aparecem em dois dígitos 5x3 lado a lado e continuam com rolagem vertical a cada mudança de segundo.

Os modos NORMAL e DUPLA permanecem inalterados.
