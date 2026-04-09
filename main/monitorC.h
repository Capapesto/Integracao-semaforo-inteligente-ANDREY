#ifndef MONITORC_H
#define MONITORC_H
 
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
 
#define SENSOR_GPIO 36

extern int carrosMt;
 

void monitoramento_init(void);
 
const char* monitorar_carro(void);
 
#endif 
 