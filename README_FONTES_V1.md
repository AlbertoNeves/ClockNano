# ClockNano — Fontes V1

Primeira versão da integração de três estilos de fonte no relógio 4 x 8x8 (32x8).

## Modos

- **NORMAL** — fonte atual Font5x7.
- **DUPLA** — fonte compacta 4x7, mais pesada, derivada da fonte normal e ajustada para 32x8.
- **SEGUNDOS** — fonte 7x8 para os dígitos do relógio, adaptada para o espaço físico de 32x8 e inspirada na `mybigfont` original.

## Menu

`MENU > FONTE` permite selecionar NORMAL, DUPLA ou SEGUNDOS.
A seleção é gravada na EEPROM no endereço 1 e é recuperada no boot.

## Importante

O ticker dos segundos ainda **não** foi implementado nesta versão. A fonte pequena 3x5 continua disponível para as telas de edição e será usada como base para a próxima etapa de animação vertical dos segundos.

## Hardware

O projeto continua usando exatamente os quatro módulos 8x8 existentes. Nenhum módulo adicional é necessário.
