# Projeto: Semáforo Inteligente com ESP32

Este projeto consiste em um sistema de controle de tráfego integrado, gerenciando 2 semáforos (veículos e pedestres), contagem de tráfego, solicitações via botão(controle de pedestres) e sinalização sonora para acessibilidade. O código foi desenvolvido integralmente por **ESP-IDF**
## Pinout(GPIO)

| Componente | Função | GPIO |
| :--- | :--- | :--- |
| **Buzzer** | Acessibilidade Sonora | 12 |
| **LED Vermelho (Carro)** | Sinalização Veicular | 22 |
| **LED Amarelo (Carro)** | Sinalização Veicular | 21 |
| **LED Verde (Carro)** | Sinalização Veicular | 19 |
| **LED Vermelho (Pedestre)** | Sinalização Pedestres | 2 |
| **LED Verde (Pedestre)** | Sinalização Pedestres | 15 |
| **Botão de Travessia** | Entrada do Pedestre | 23 |
| **Sensor de Presença** | Contador de Carros | 13 |

## Módulos

### Semáforo Veicular (`semafC.h/c`)
Gerencia os estados principais do trânsito:
* **Estado 0 (Vermelho):** Bloqueio de veículos. O tempo de espera é dinâmico:
    * Se o botão for pressionado: Tempo reduzido pela metade.
    * Se houver acúmulo de carros (>= 5): Tempo reduzido proporcionalmente à quantidade de carros.
* **Estado 1 (Verde):** Fluxo livre para veículos.
* **Estado 2 (Amarelo):** Atenção/Transição.

### 2. Semáforo de Pedestres (`semafP.h/c`)
Sincronizado com o sinal veicular:
* **Carro Vermelho:** Pedestre recebe sinal Verde.
* **Alerta:** Quando faltar menos de 1 segundo para o sinal veicular abrir, o LED vermelho do pedestre pisca rapidamente (250ms).
* **Carro Verde/Amarelo:** Pedestre recebe sinal Vermelho fixo.

### 3. Acessibilidade (`acessibilidadeS.c/h`)
Emite sinais sonoros via buzzer baseados no estado veicular:
* **Verde:** Bipes de 500ms.
* **Amarelo:** Bipes rápidos de 200ms.
* **Vermelho:** Silêncio para segurança na travessia.

### 4. Monitoramento e Botão
* **Contador:** Incrementa a variável `carrosMt` ao detectar nível lógico alto no `SENSOR_GPIO`. - (`monitorC/h`)
* **Botão (TRFB):** Altera a flag `TRF_3_BOTAO` para interferir na lógica de tempo do semáforo veicular. (`trfb.c/h`)

##  Prioridades

1.  **Controle de Botão (TRFB):** 6 (Mais alta)
2.  **Semáforo Veicular:** 5
3.  **Semáforo Pedestre:** 4
4.  **Acessibilidade:** 3


## Materiáis

| Item | Qtd. |
| ESP32 DevKit V1	| 1 |
| Módulo LDR | 1 |
| LED Vermelho | 2 |
| LED Amarelo | 1 |
| LED Verde | 2 |
| Resistor 220Ω | 5 |



## Créditos e Autoria
* **Integração e Remake de todos + Semáforo Veículos:** Andrey André
* **Acessibilidade:** Lucas Espanhol
* **Monitoramento:** Thiago Albuquerque e Zaratim
* **Pedestres:** João P. Borges e João V. Moreno
* **Botão/Tráfego:** Arthur Rabelo
