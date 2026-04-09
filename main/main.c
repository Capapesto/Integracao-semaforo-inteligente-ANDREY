#include "monitorC.h"
#include "semafC.h"
#include "semafP.h"
#include "TRFB.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "acessibilidadeS.h"

void app_main(void) {
    
    semaforo_iniciar(5); 
    semP_iniciar(4);
    trfb_init(6);
    monitoramento_init();
    acessS_init(3);

    while(1) {
             
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}