#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Dependências externas (Vêm do semafC.h/main)
extern int semC_estado_atual;
extern int semC_tempo_restante;

// Definição de Pinos - Pedestre
#define SemP_RED 27
#define SemP_Green 18

/* LÓGICA DO SEMÁFORO DE PEDESTRES:
  - Se Carro = VERMELHO (0) e tempo > 1s: Pedestre AVANÇA (Verde)
  - Se Carro = VERMELHO (0) e tempo < 1s: Pedestre CUIDADO (Pisca Vermelho)
  - Se Carro = VERDE (1) ou AMARELO (2): Pedestre PARE (Vermelho Fixo)
*/

static void vTaskSemP(void *pvParameters) {
    // Configuração inicial
    gpio_reset_pin(SemP_RED);
    gpio_set_direction(SemP_RED, GPIO_MODE_OUTPUT);
    gpio_reset_pin(SemP_Green);
    gpio_set_direction(SemP_Green, GPIO_MODE_OUTPUT);

    while (true) {
        // CASO 1: Carros parados (Vermelho) - Pedestre pode passar
        if (semC_estado_atual == 0) {
            
            // Sub-caso: O tempo dos carros está acabando (menos de 1 segundo)
            if (semC_tempo_restante < 1000) {
                printf("PEDESTRE: Cuidado! Sinal vai fechar.\n");
                // Pisca o Vermelho rapidamente
                gpio_set_level(SemP_Green, 0);
                gpio_set_level(SemP_RED, 1);
                vTaskDelay(pdMS_TO_TICKS(250));
                gpio_set_level(SemP_RED, 0);
                vTaskDelay(pdMS_TO_TICKS(250));
            } 
            // Sub-caso: Tempo seguro para travessia
            else {
                printf("PEDESTRE: Avance.\n");
                gpio_set_level(SemP_RED, 0);
                gpio_set_level(SemP_Green, 1);
                vTaskDelay(pdMS_TO_TICKS(500)); // Checa a cada meio segundo
            }
        } 
        // CASO 2: Carros em movimento ou atenção (Verde ou Amarelo)
        else {
            printf("PEDESTRE: Pare.\n");
            gpio_set_level(SemP_Green, 0);
            gpio_set_level(SemP_RED, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

void semP_iniciar(int prioridade) {
    xTaskCreate(
        vTaskSemP,
        "SemP_Task", 
        2048, 
        NULL, 
        prioridade, 
        NULL
    );
}
