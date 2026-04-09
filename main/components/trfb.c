#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "semafC.h"
/*
FEITO POR:
Arthur Rabelo
Andrey André (Remake + integração)
*/

#define BOTAO_PIN  21
int TRF_3_BOTAO = 0;

static void vTaskTRFB(void *pvParameters) {
    

    gpio_reset_pin(BOTAO_PIN);
    gpio_set_direction(BOTAO_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOTAO_PIN, GPIO_PULLDOWN_ONLY);

    printf("'trf'...\n");

    while (1) {
        int botao = gpio_get_level(BOTAO_PIN);

        
        if (botao == 1 && semC_estado_atual == 0){
            TRF_3_BOTAO = 1;    
        } 
        else if(botao == 0 && semC_estado_atual == 0 && TRF_3_BOTAO ==  1){
              TRF_3_BOTAO = 1;
        }
        else if (botao == 1 && semC_estado_atual ==0){
            TRF_3_BOTAO = 0;
        }
        else if (botao == 0 && semC_estado_atual == 0) {
            TRF_3_BOTAO =0;
        }
        else {
            TRF_3_BOTAO = TRF_3_BOTAO;
        }


        printf("Semaforo: %d | Botao: %d | trf: %d\n", semC_estado_atual, botao, TRF_3_BOTAO);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


 void trfb_init(int prioridade) {
    xTaskCreate(
        vTaskTRFB,
        "trfb_Task", 
        2048, 
        NULL, 
        prioridade, 
        NULL
    );
}
