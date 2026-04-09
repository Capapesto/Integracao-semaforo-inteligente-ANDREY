/* ==========================================================================
 * INCLUDES 
 * ========================================================================== */
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Headers do projeto
//#include "semafC.h"
//#include "semafP.h"
//#include "monitoramento.h"
//#include "monitorC.h"
//#include "trfb.h"
//#include "acessibilidadeS.h"

/* ==========================================================================
 * DEFINES (Pinos)
 * ========================================================================== */
// Acessibilidade
#define BUZZER_GPIO 12

// Semáforo Carros (semafC)
#define semC_RED  22
#define semC_YLW  21
#define semC_GRN  19

// Semáforo Pedestres (semafP)
#define SemP_RED 2
#define SemP_Green 15

// Botão / Tráfego
#define BOTAO_PIN  23

// Contador
#define SENSOR_GPIO 13

/* ==========================================================================
 * VARIÁVEIS GLOBAIS
 * ========================================================================== */
// Monitoramento
int carrosMt = 0;              
static char mensagem[64];            

// Semáforo Carros
int semC_tempo_restante = 0;
int semC_estado_atual = 0;
int semC_time_template = 3000;

// Tráfego / Botão
int TRF_3_BOTAO = 0;

/* ==========================================================================
 * MÓDULO: ACESSIBILIDADE
 * FEITO POR: Lucas Espanhol, Andrey André (remake)
 * ========================================================================== */

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

/* ==========================================================================
 * MÓDULO: MONITORAMENTO
 * FEITO POR: Thiago Albuquerque e zaza, INTEGRADO POR: Andrey André
 * ========================================================================== */

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

/* ==========================================================================
 * MÓDULO: SEMÁFORO CARROS
 * FEITO E INTEGRADO POR ANDREY
 * ========================================================================== */

// Pode ser alterado por outras tasks
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

/* ==========================================================================
 * MÓDULO: SEMÁFORO PEDESTRES
 * FEITO POR: João P. Borges, João V. Moreno, Andrey André (Remake + integração)
 * ========================================================================== */

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

/* ==========================================================================
 * MÓDULO: CONTROLE DE BOTÃO (TRFB)
 * FEITO POR: Arthur Rabelo, Andrey André (Remake + integração)
 * ========================================================================== */

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
        else if ((botao == 1 && semC_estado_atual ==0)){
            TRF_3_BOTAO = 0;
        }
        else if (botao == 0 && semC_estado_atual == 0) {
            TRF_3_BOTAO =0;
        }
        else {
            TRF_3_BOTAO = 0;
        }

        printf("Semaforo: %d | Botao: %d | trf: %d | Carros: %d |\n", semC_estado_atual, botao, TRF_3_BOTAO, carrosMt);

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

/* ==========================================================================
 * MAIN
 * ========================================================================== */

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
