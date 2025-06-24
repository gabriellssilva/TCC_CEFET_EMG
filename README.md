
# 📡 Projeto TCC para Aquisição de Sinais EMG com ESP32

Este projeto utiliza um microcontrolador ESP32 para realizar a leitura e análise de sinais EMG (Eletromiografia) de quatro músculos distintos: **supinador**, **pronador**, **extensor dos dedos** e **flexor dos dedos**.

## 🧠 Objetivo

Desenvolver um sistema embarcado para aquisição, processamento e transmissão de sinais EMG, permitindo análise em tempo real dos músculos do antebraço.

## ⚙️ Componentes Utilizados

- **ESP32**
- **Sensores EMG **
- Fios jumper
- Protoboard ou circuito dedicado
- Fonte de alimentação

## 📌 Mapeamento de Pinos

| Músculo               | Pino AD no ESP32 |
|-----------------------|------------------|
| Supinador             | 13               |
| Pronador (Bíceps)     | 14               |
| Extensor de dedos     | 27               |
| Flexor de dedos       | 26               |

## 📊 Funcionalidades

- Coleta de sinais EMG em tempo real.
- Cálculo de **RMS** (Root Mean Square) para análise da intensidade do sinal.
- Armazenamento de amostras recentes para cada músculo.
- Possível integração com aplicações de reabilitação, próteses ou interfaces neurais.

## 📦 Estrutura do Código

- **Buffers** para armazenar amostras de cada músculo (`TAMANHO` e `TAMANHO_RMS`).
- **Cálculo de RMS** para cada grupo muscular.
- Possibilidade de enviar os dados via serial ou outro protocolo (ex: Bluetooth, Wi-Fi – dependendo da continuação do código).

## ▶️ Como Usar

1. Conecte os sensores EMG conforme o mapeamento de pinos.
2. Faça o upload do código para o ESP32 usando a IDE Arduino.
3. Abra o monitor serial para observar os sinais EMG ou integre com seu software de leitura de dados.

## 🧪 Possíveis Aplicações

- Controle de próteses mioelétricas
- Sistemas de reabilitação muscular
- Interfaces cérebro-máquina
- Jogos e realidade aumentada com controle muscular
