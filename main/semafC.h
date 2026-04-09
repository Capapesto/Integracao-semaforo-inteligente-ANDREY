#ifndef SEMAFC_H
#define SEMAFC_H
void semaforo_iniciar(int prioridade);
extern int semC_estado_atual;
extern int semC_time_latch;
extern int semC_time_template;
extern int semC_estado_atual;
extern int semC_tempo_restante; // Nova variável para o contador
extern int TRF_3_BOTAO;
extern int carrosMt;
#endif