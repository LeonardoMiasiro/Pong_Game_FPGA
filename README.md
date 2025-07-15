# 🏓 Pong Game na DE1-SoC

Este projeto é uma versão do clássico jogo **Pong**, implementada na placa **DE1-SoC** da Altera (Intel) utilizando a linguagem C. O jogo usa o VGA como saída gráfica e os botões físicos da placa para controlar as barras dos jogadores.

## 🎮 Sobre o jogo

Dois jogadores controlam barras verticais que podem se mover para cima ou para baixo utilizando os botões KEY da FPGA:

- **Player 1:** KEY0 (sobe) e KEY1 (desce)
- **Player 2:** KEY2 (sobe) e KEY3 (desce)

A bola se move automaticamente pela tela e rebate nas bordas e nas barras dos jogadores. Cada vez que um jogador falha em rebater a bola, o adversário marca um ponto. O placar é exibido no canto superior da tela.

## 🛠️ Tecnologias e ferramentas utilizadas

- **Linguagem C**
- **Quartus Prime 18.1**
- **FPGA Monitor Program**
- **Placa DE1-SoC**
- Saída gráfica via **VGA**
- Leitura dos botões via periféricos mapeados em memória

## ▶️ Como rodar

1. Instale o **Quartus Prime 18.1**.
2. Instale o **FPGA Monitor Program** dentro da pasta do Quartus.
3. Conecte a **placa DE1-SoC** via cabo USB e conecte um **monitor VGA** à placa.
4. Abra o **FPGA Monitor Program**.
5. Importe o código C do jogo Pong.
6. Compile e envie o programa para a placa.
7. O jogo será exibido no monitor.
8. Use os botões para controlar as barras e jogar!

## 📷 Captura de tela

(Adicione aqui uma imagem do jogo em execução, se disponível.)

## 👨‍💻 Autores

- **Leonardo Ryuiti Miasiro**
- **Pedro Marchi Nunes**

Estudantes de **Engenharia da Computação** na **Universidade Federal de São Carlos (UFSCar)**.

Este projeto foi desenvolvido no **Departamento de Computação da UFSCar**, como trabalho prático da disciplina **Arquitetura de Alto Desempenho**, ministrada pelo professor **Emerson Carlos Pedrino**.