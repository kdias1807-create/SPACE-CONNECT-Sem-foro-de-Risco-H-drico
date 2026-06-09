# 🌊 SPACE CONNECT — Semáforo de Risco Hídrico

> Monitoramento de enchentes com Arduino e tecnologia espacial  
> FIAP · Global Solution 2026 · Edge Computing & Computer Systems

---

## 👥 Integrantes

| Nome | RM |
|------|----|
| Kaue Fernando Jaques Lopes | 571353 |
| Matheus Martins Santos | 570884 |
| Leonardo Gonçalves Cardoso da Fonseca | 569475 |
| Kaio Nincao Maia Dias | 568834 |

---

## 📋 Descrição do Projeto

O **Space Connect** é uma solução de monitoramento hídrico desenvolvida como parte da Global Solution 2026 da FIAP, com foco no tema **Indústria Espacial aplicada a desastres naturais**.

O projeto simula um sistema embarcado de alerta de enchentes que utiliza sensores para monitorar o nível de chuva e as condições climáticas em tempo real, exibindo o risco em um painel visual (semáforo de LEDs) e em um display LCD — conectando a ideia ao ecossistema do Space Connect, que consome dados reais do satélite SWOT da NASA para antecipar enchentes com 24 a 72 horas de antecedência.

---

## 🎯 Objetivo da Solução

Demonstrar como um dispositivo de borda (edge device) baseado em Arduino pode ser utilizado como ponto de visualização local de alertas hídricos, complementando sistemas de monitoramento por satélite como o Space Connect.

O sistema permite que gestores públicos e equipes da Defesa Civil visualizem o nível de risco de enchente em tempo real, com indicação visual clara e mensagens de ação recomendada — sem depender de conexão com a internet.

---

## 🔧 Componentes Utilizados

| Componente | Quantidade | Função |
|------------|-----------|--------|
| Arduino Uno | 1 | Microcontrolador principal |
| Potenciômetro (simula sensor de chuva) | 1 | Leitura analógica do nível de chuva |
| Sensor DHT22 | 1 | Leitura de temperatura e umidade |
| Display LCD 16x2 (I2C) | 1 | Exibição de dados e mensagens |
| LED Verde | 1 | Indicador — Nível NORMAL |
| LED Amarelo | 1 | Indicador — Nível ATENÇÃO |
| LED Laranja | 1 | Indicador — Nível ALERTA |
| LED Vermelho | 1 | Indicador — Nível CRÍTICO (pisca) |
| Resistores 220Ω | 4 | Proteção dos LEDs |
| Pushbutton (azul) | 1 | Alternância entre telas do LCD |
| Protoboard + jumpers | — | Conexões do circuito |

---

## ⚙️ Explicação do Funcionamento

O sistema lê continuamente o sensor de chuva (porta A0) e o sensor DHT22 (porta D2). Com base na leitura, classifica o risco em **4 níveis**:

| Nível | Leitura (0–1023) | LEDs acesos | Ação recomendada |
|-------|-----------------|-------------|------------------|
| 🟢 NORMAL   | > 750 | Verde | Monitorar rotineiramente |
| 🟡 ATENÇÃO  | 500–750 | Verde + Amarelo | Verificar setores vulneráveis |
| 🟠 ALERTA   | 250–500 | Verde + Amarelo + Laranja | Acionar equipes de resposta |
| 🔴 CRÍTICO  | < 250 | Todos + Vermelho piscando | EVACUAR zonas de risco |

O **display LCD** alterna entre 3 telas pelo botão azul:
- **Tela 1** — Porcentagem de chuva e nível de risco
- **Tela 2** — Temperatura (°C) e umidade relativa (%)
- **Tela 3** — Mensagem de ação recomendada para o gestor

O **Serial Monitor** exibe logs a cada 2 segundos com todos os dados lidos.

---

## 🔌 Estrutura do Circuito

```
Arduino Uno
├── A0          → Potenciômetro (SIG) — sensor de chuva
├── D2          → DHT22 (DATA)
├── A4          → LCD SDA (I2C)
├── A5          → LCD SCL (I2C)
├── D3          → Resistor 220Ω → LED Verde
├── D4          → Resistor 220Ω → LED Amarelo
├── D5          → Resistor 220Ω → LED Laranja
├── D6          → Resistor 220Ω → LED Vermelho
├── D7          → Botão NEXT (INPUT_PULLUP)
├── 5V          → VCC (Pot, DHT22, LCD)
└── GND         → GND (todos os componentes)
```

---

## 🚀 Instruções de Execução

### Simulação no Wokwi

1. Acesse [wokwi.com](https://wokwi.com) e crie um novo projeto **Arduino Uno**
2. Substitua o conteúdo do arquivo `diagram.json` pelo arquivo disponível neste repositório
3. Substitua o código do editor pelo arquivo `sketch_semaforo.ino`
4. Clique em **▶ Play** para iniciar a simulação
5. Gire o **potenciômetro** para simular o aumento do nível de chuva
6. Pressione o **botão azul** para alternar entre as telas do LCD
7. Observe os LEDs mudando conforme o nível de risco sobe

### Implementação Física (opcional)

1. Monte o circuito conforme o diagrama acima em uma protoboard
2. Conecte o Arduino ao computador via USB
3. Abra a Arduino IDE e instale as bibliotecas:
   - `LiquidCrystal_I2C` (by Frank de Brabander)
   - `DHT sensor library` (by Adafruit)
4. Faça o upload do arquivo `sketch_semaforo.ino`
5. Abra o **Serial Monitor** (9600 baud) para acompanhar os logs

---

## 📁 Arquivos do Repositório

```
├── sketch_semaforo.ino     → Código fonte Arduino
├── diagram_semaforo.json   → Diagrama do circuito (Wokwi)
└── README.md               → Este arquivo
```

---

## 🔗 Contexto — Space Connect

Este projeto faz parte do ecossistema **Space Connect**, solução completa de monitoramento de enchentes que integra:

- 🛰️ Dados reais do satélite **SWOT (NASA/CNES)**
- 🔗 Coleta via **earthaccess** e **Hydrocron API**
- 🗄️ Armazenamento geoespacial em **PostGIS**
- 📊 Dashboard com mapas de risco e alertas automáticos
- ⚡ **Este dispositivo Arduino** como ponto de alerta local (edge)

---

*FIAP · Global Solution 2026 · Edge Computing & Computer Systems*  
*Prof. Paulo Marcotti · Prof. Lucas Demetrius*
