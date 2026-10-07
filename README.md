CLOCKNANO
Manual de Usuário
Versão funcional validada — outubro de 2026
O ClockNano é um relógio digital baseado em Arduino Nano, com display de LEDs controlado por MAX7219, RTC, alarmes, melodias e gerenciamento automático da exibição conforme a presença do carregador.
1. Visão geral
Enquanto o carregador está conectado, o display funciona normalmente. Quando o carregador é retirado, o relógio continua funcionando internamente, mas a exibição é desligada para economizar energia. O botão HOME permite acordar temporariamente o display por 30 segundos.
2. Controles e conexões
Controle	Pino	Função
HOME	D5	Acesso/retorno principal e, sem carregador, acorda o display por 30 s.
Botão −	D2	Diminui valores ou navega para baixo, conforme a tela.
Botão OK	D3	Confirma seleção, conforme a tela.
Botão +	D4	Aumenta valores ou navega para cima, conforme a tela.
Buzzer	D6	Reprodução dos sons de alarme.
Detecção do carregador	D12	HIGH = carregador presente; LOW = carregador ausente.
MAX7219 DIN	D11	Dados para o display.
MAX7219 CLK	D13	Clock do display.
MAX7219 CS	D10	Seleção do display.
3. Display e carregador
3.1 Carregador conectado
Com D12 em HIGH, o display permanece ligado e o relógio funciona normalmente.
3.2 Carregador desconectado
Com D12 em LOW, o MAX7219 entra em SHUTDOWN e os LEDs ficam apagados. O RTC, relógio, alarmes e demais funções continuam funcionando.
3.3 Acordar o display
Sem carregador, pressione HOME. O display liga por 30 segundos. Se HOME for pressionado novamente durante esse período, os 30 segundos são reiniciados. Ao terminar o período, o display volta a desligar.
4. Operação básica
O ClockNano apresenta a hora durante a operação normal. Os botões são contextuais: suas funções podem mudar conforme a tela ou menu. Em configurações, use os botões de incremento/decremento para alterar valores e o botão de confirmação para avançar ou confirmar. HOME permite retornar ao nível principal.
5. Ajuste do RTC
O relógio utiliza um RTC para manter data e hora. Entre na opção de configuração do RTC, altere os campos apresentados e confirme a operação. A nova data e hora passam a ser utilizadas pelo relógio e pelos alarmes.
6. Alarmes
Os alarmes possuem horário, dias de repetição e uma melodia associada, conforme as opções disponíveis no firmware. Quando o horário programado é atingido, o buzzer reproduz a melodia.
6.1 SNOOZE / HOME
HOME atua como SNOOZE/controle do alarme quando o alarme está ativo. Fora dessa situação, sem carregador, HOME também acorda o display por 30 segundos.
7. Melodias
O ClockNano possui uma biblioteca interna de melodias. Foi incluído o alarme clássico de três bipes repetidos — “pi pi pi, pi pi pi...” — com duração aproximada de 20 segundos, podendo ser interrompido pelo SNOOZE/HOME.
8. Comportamento resumido
Condição	D12	Display
Carregador conectado	HIGH	Ligado
Carregador desconectado	LOW	Desligado
HOME sem carregador	LOW	Ligado por 30 s
HOME novamente antes de 30 s	LOW	Reinicia os 30 s
Fim dos 30 s	LOW	Desligado
9. Cuidados de uso
Não aplique tensões externas aos pinos de I/O sem verificar a compatibilidade com o Arduino Nano. A detecção do carregador em D12 recebe 5 V através de resistor de 10 kΩ.
O buzzer utiliza D6 e possui GND próprio. D7 não é mais utilizado como GND artificial.
10. Informações técnicas para manutenção
Plataforma: Arduino Nano / ATmega328P. Display: matriz de LEDs com MAX7219. O display usa D11 (DIN), D13 (CLK) e D10 (CS). D12 é usado como entrada de detecção do carregador. O desligamento usa o recurso SHUTDOWN do MAX7219.
O temporizador de 30 segundos usa millis(), sem delay de 30 segundos; portanto, o relógio, RTC e alarmes continuam sendo processados.
11. Solução rápida de problemas
Sintoma	Verificação
Display não acende com carregador	Verifique alimentação, MAX7219 e se D12 está em HIGH.
Display não apaga sem carregador	Meça D12; o firmware espera LOW.
HOME não acorda o display	Verifique o botão HOME em D5 e a condição sem carregador.
Display apaga imediatamente após HOME	Verifique D12 e o controle SHUTDOWN do MAX7219.
Alarme não soa	Verifique buzzer em D6 e configuração do alarme/melodia.
12. Versão de referência
A referência funcional deste manual é o projeto ClockNano300926, validado com o controle automático do display por D12, despertar por HOME durante 30 segundos e o alarme clássico “pi pi pi” na biblioteca de melodias.
ClockNano — Manual de Usuário
