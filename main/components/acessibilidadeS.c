#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "semafC.h"
//semC_estado_atual;
#define BUZZER_GPIO 17

/*
FEITO POR:
Lucas Espanhol
Andrey André  (remake)
*/

void setup_gpio(int pin) {
    esp_rom_gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

static void vTaskAcess(void *pvParameters) 
{
    // Configuração dos pinos
    setup_gpio(BUZZER_GPIO);
  
    while (1)
    {
        // ===== MODO VERDE =====
        if ( semC_estado_atual == 1){
            gpio_set_level(BUZZER_GPIO, 1);
            vTaskDelay(pdMS_TO_TICKS(500));

            gpio_set_level(BUZZER_GPIO, 0);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

      

        // ===== MODO AMARELO =====


       if ( semC_estado_atual == 2){
            gpio_set_level(BUZZER_GPIO, 1);
            vTaskDelay(pdMS_TO_TICKS(200));

            gpio_set_level(BUZZER_GPIO, 0);
            vTaskDelay(pdMS_TO_TICKS(200));
        }


        // ===== MODO VERMELHO  =====
        if ( semC_estado_atual == 1){
     
        gpio_set_level(BUZZER_GPIO, 0);

        vTaskDelay(pdMS_TO_TICKS(5000));

    }
}}

 void acessS_init(int prioridade) {
    xTaskCreate(
        vTaskAcess,
        "acess_Task", 
        2048, 
        NULL, 
        prioridade, 
        NULL
    );
}


