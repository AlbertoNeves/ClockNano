# ClockNano V12 — refinamento do espaçamento da fonte NORMAL

A V12 altera somente o layout da tela principal quando a fonte NORMAL está selecionada.

Layout físico solicitado:
- dezena da hora: coluna 1
- 1 coluna apagada
- unidade da hora: coluna 7
- 2 colunas apagadas
- dois-pontos: coluna 14 (glyph 5x7)
- 2 colunas apagadas
- dezena dos minutos: coluna 21
- 1 coluna apagada
- unidade dos minutos: coluna 27

A soma ocupa exatamente as 32 colunas do display.

NORMAL, DUPLA e SEGUNDOS mantêm as fontes e comportamentos da V11; a animação dos segundos não foi alterada.


## V14 — NORMAL refinada

A tela NORMAL teve apenas o posicionamento dos dígitos refinado.
O ':' permanece em X=14. Os dígitos passaram para X=3/9 e X=19/25, aproximando HH e MM do separador e centralizando visualmente o conjunto.

DUPLA e SEGUNDOS permanecem inalterados.
