#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "semafC.h"



/*
FEITO POR:
João P. Borges
João V. Moreno
Andrey André (Remake + integração)
*/ 

//SEMAFp.C
// definir pinos
#define SemP_RED 27
#define SemP_Green 18



static void vTaskSemP(void *pvParameters) {

  gpio_set_direction(SemP_RED, GPIO_MODE_OUTPUT);
  gpio_set_direction(SemP_Green, GPIO_MODE_OUTPUT);


int SemP_vezes =  5;

  while (true) {
     

    if (semC_estado_atual == 0 && semC_tempo_restante < 1000){
    
    printf("cuidado\n");
      for (int i = 0; i < SemP_vezes; i++) {
        gpio_set_level(SemP_RED, 1);
        gpio_set_level(SemP_Green, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(SemP_RED, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
} 
    else if (semC_estado_atual == 1) {
   
     printf("pare\n");
      gpio_set_level(SemP_RED, 1);
      gpio_set_level(SemP_Green, 0);
      vTaskDelay(5000 / portTICK_PERIOD_MS);
} 
    else if (semC_estado_atual == 0) {
    
     printf("avance\n");
     gpio_set_level(SemP_RED, 0);
     gpio_set_level(SemP_Green, 1);
     vTaskDelay(3000 / portTICK_PERIOD_MS);
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
