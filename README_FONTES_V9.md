# ClockNano — V9: dig6x8 + dig3x7

Esta versão parte da V8 funcional e testa duas fontes do arquivo `fonts.h` no hardware 4x MAX7219 (32x8):

## FONTE DUPLA

A opção **DUPLA** passa a usar a `dig6x8` original do arquivo `fonts.h`:

- 6 colunas x 8 linhas por dígito;
- quatro dígitos ocupam 24 colunas;
- `:` compacto ocupa 2 colunas;
- layout total de 26 colunas, centralizado no display 32x8;
- altura total de 8 pixels.

## FONTE SEGUNDOS

A opção **SEGUNDOS** passa a usar `dig3x7`:

- 3 colunas x 7 linhas;
- dois dígitos ocupam 7 colunas com 1 coluna de separação;
- mantém o relógio principal NORMAL 5x7;
- mantém a animação tipo odômetro da V8;
- 37 -> 38: somente unidade;
- 39 -> 40: os dois dígitos;
- 59 -> 00: os dois dígitos.

Os dados de `dig3x7` foram normalizados de bits 1..7 para bits 0..6, preservando o desenho original para a rotina de desenho do Canvas.

## O que não foi alterado

- NORMAL permanece Font5x7.
- Menu e EEPROM permanecem com NORMAL / DUPLA / SEGUNDOS.
- Display físico permanece 32x8.
- RTC e demais telas permanecem inalterados.
- `Font5x3` continua disponível para telas de edição; somente deixou de ser usada pelo indicador de segundos.

## Teste recomendado

Após compilar e gravar, testar:

- NORMAL;
- DUPLA, especialmente 00:00, 12:34, 23:59;
- SEGUNDOS, observando 09->10, 19->20, 29->30, 39->40, 49->50 e 59->00.

Esta versão é para avaliação visual no hardware. Não fazer commit antes de aprovar as duas novas fontes.
