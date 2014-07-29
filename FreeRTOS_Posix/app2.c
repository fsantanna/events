/*
 * Exemplo de uma aplicação.
 */

#include "app.h"

/* Inclui as extenções utilizadas na aplicação. */

#ifdef EXT_TIMER
#include "ext_timer.h"
#endif

void panic (int err) {
    printf("ERR = %d\n", err);
}

void cb_timer_expired (evt_param_t param) {
    printf("cb_timer_expired\n");
    evt_queue_put(EVT_TIMER_OUT_START, (evt_param_t)1000, 0);
}

void cb_timer_busy (evt_param_t param) {
    printf("cb_timer_busy\n");
}

void app_init (void)
{
    /* inicializa a biblioteca de eventos com uma função de pânico */
    evt_init(panic);

    /* inicializa as extensões utilizadas */
#ifdef EXT_TIMER
    ext_timer_init();
#endif

    /* Cria a aplicação, i.e., interliga as extensões que passam a se
     * comunicar através de eventos: */

    /*
     * Teste de Timer:
     * Esse driver básico espera um "OUT_START" com um valor em milisegundos e 
     * gera um "IN_EXPIRED" quando o tempo expirar.
     * Caso um outro "OUT_START" seja emitido, o driver gera um "IN_BUSY".
     */
    evt_listener_add(EVT_TIMER_IN_EXPIRED, cb_timer_expired);
    evt_listener_add(EVT_TIMER_IN_BUSY,    cb_timer_busy);
    evt_queue_put(EVT_TIMER_OUT_START, (evt_param_t)1000, 0);
    evt_queue_put(EVT_TIMER_OUT_START, (evt_param_t)2000, 0);   /* busy */
}
