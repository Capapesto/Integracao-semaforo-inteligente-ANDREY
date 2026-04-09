#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "trfb.h"
#include "monitorC.h"


//SEMAFc.C

// FEITO E INTEGRADO POR ANDREY


// Definições internas
#define semC_RED  23
#define semC_YLW  22
#define semC_GRN  19
int semC_tempo_restante = 0;
int semC_estado_atual = 0;
int semC_time_template = 3000;
 // Pode se= 1; r int TRF_3_BOTAOalterado por outras tasks

void inicializar_pinos() {
    gpio_reset_pin(semC_RED);
    gpio_set_direction(semC_RED, GPIO_MODE_OUTPUT);
    gpio_reset_pin(semC_YLW);
    gpio_set_direction(semC_YLW, GPIO_MODE_OUTPUT);
    gpio_reset_pin(semC_GRN);
    gpio_set_direction(semC_GRN, GPIO_MODE_OUTPUT);
}

void semC_time_latch(int divisor) {
    
    int tempo_total = semC_time_template / divisor;
    semC_tempo_restante = tempo_total;

    while (semC_tempo_restante > 0) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        semC_tempo_restante -= 100; // Diminui 100ms a cada volta
    }
}

//TASK
static void vTaskSemaforo(void *pvParameters) {
    printf("Iniciando Task do Semáforo...\n");
    inicializar_pinos();

    while (1) {
        switch(semC_estado_atual) {
            case 0: // RED
                gpio_set_level(semC_RED, 1);
                gpio_set_level(semC_YLW, 0);
                gpio_set_level(semC_GRN, 0);
                
                if (TRF_3_BOTAO == 1) semC_time_latch(2);
                else if ((carrosMt >= 5))   semC_time_latch(carrosMt/2);

                else semC_time_latch(1);
                
                semC_estado_atual = 1;
                break;

            case 1: // GRN
                gpio_set_level(semC_RED, 0);
                gpio_set_level(semC_YLW, 0);
                gpio_set_level(semC_GRN, 1);
                semC_time_latch(1);
                semC_estado_atual = 2;
                break;

            case 2: // YLW
                gpio_set_level(semC_RED, 0);
                gpio_set_level(semC_YLW, 1);
                gpio_set_level(semC_GRN, 0);
                semC_time_latch(3);
                semC_estado_atual = 0;
                //67
                break;
        }
    }
}
// 67
void semaforo_iniciar(int prioridade) {
    xTaskCreate(
        vTaskSemaforo, 
        "Semaforo_Task", 
        2048, 
        NULL, 
        prioridade, 
        NULL
    );
}