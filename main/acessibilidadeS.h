#ifndef ACESSIBILIDADES_H
#define ACESSIBILIDADES_H
 
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
 
#define BUZZER_GPIO 17

extern int semC_estado_atual;
 

void acessS_init(int prioridade);
 
#endif 
 