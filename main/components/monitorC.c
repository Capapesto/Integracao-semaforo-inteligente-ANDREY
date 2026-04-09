#include "monitoramento.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
/*

FEITO POR:
Thiago Albuquerque

INTEGRADO POR:
Andrey André

*/
int carrosMt = 0;              
static char mensagem[64];           
 
void monitoramento_init(void) {
    gpio_set_direction(SENSOR_GPIO, GPIO_MODE_INPUT);
}
 
const char* monitorar_carro(void) {
    if (gpio_get_level(SENSOR_GPIO) == 1) {
        carrosMt++;
        snprintf(mensagem, sizeof(mensagem),
                 "Carro detectado! Total: %d carro(s)", carrosMt);
 
        
        while (gpio_get_level(SENSOR_GPIO) == 1) {
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
 
        return mensagem;  
    }
 
    return NULL;  
}