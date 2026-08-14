# ClockNano — V10

## Alterações

### Fonte DUPLA
- Mantida a `dig6x8` aprovada na V9.
- Adicionada 1 coluna de espaço entre os dois dígitos da hora.
- Adicionada 1 coluna de espaço entre a hora e os dois pontos.
- Adicionada 1 coluna de espaço entre os dois pontos e os minutos.
- Adicionada 1 coluna de espaço entre os dois dígitos dos minutos.
- Layout total: 30 colunas, centralizado no display 32x8 com 1 coluna livre em cada lado.

### Segundos
- Retornada a fonte `Font5x3` (3x5).
- Mantida a animação odômetro da V8: somente os dígitos que mudam rolam.
- Mantida a separação de 1 linha apagada entre o dígito que sai e o que entra.
- Indicador SS continua ocupando 7 colunas: 3 + 1 + 3.
- Fonte 3x7 deixa de ser utilizada, mas o arquivo pode permanecer no projeto para testes futuros.

NORMAL permanece inalterada.
