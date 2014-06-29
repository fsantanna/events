/*
 * Exemplo de uma aplicação.
 */

#include "app.h"

/* Inclui as extenções utilizadas na aplicação. */

#include "ext_timer.h"
#include "ext_lua.h"

int app_ret = 1;

void panic (int err) {
    printf("ERR = %d\n", err);
    app_ret *= 2;
}

void cb_2 (evt_param_t param) {
    printf("cb_2 = %d\n", param.v);
    app_ret += 2*param.v;
}

void cb_1 (evt_param_t param) {
    printf("cb_1 = %d\n", param.v);
    app_ret += param.v;
    if (param.v == 2) {
        evt_listener_rem(0, cb_1);
        evt_listener_add(2, cb_2);
    }
}

void cb_timer_expired (evt_param_t param) {
    printf("cb_timer_expired\n");
    //evt_queue_put(EVT_QUIT, (evt_param_t)0, 0);
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
#ifdef EXT_LUA
    ext_lua_init();
#endif

    /* Cria a aplicação, i.e., interliga as extensões que passam a se
     * comunicar através de eventos: */

    /* Teste básico:
     *  - escuta "EVT_TEST" (listener_add)
     *  - gera uma instância do evento (queue_put)
     */
    evt_listener_add(EVT_TEST, cb_1);
    evt_queue_put(EVT_TEST, (evt_param_t)1, 0);
    evt_queue_put(EVT_TEST, (evt_param_t)2, 0);
    evt_queue_put(EVT_TEST, (evt_param_t)3, 0);
    evt_queue_put(EVT_TEST, (evt_param_t)NULL, 100000);
                            /* teste de estouro da fila de eventos */

    /*
     * Teste de Timer:
     * Esse driver básico espera um "OUT_START" com um valor em milisegundos e 
     * gera um "IN_EXPIRED" quando o tempo expirar.
     * Caso um outro "OUT_START" seja emitido, o driver gera um "IN_BUSY".
     */
    evt_listener_add(EVT_TIMER_IN_EXPIRED, cb_timer_expired);
    evt_listener_add(EVT_TIMER_IN_BUSY,    cb_timer_busy);
    evt_queue_put(EVT_TIMER_OUT_START, (evt_param_t)2000, 0);
    evt_queue_put(EVT_TIMER_OUT_START, (evt_param_t)2000, 0);   /* busy */

    /*
     * Teste de Lua:
     * Solicita que o "driver" de Lua execute o arquivo passado.
     */
    evt_queue_put(EVT_LUA_OUT_FILE, (evt_param_t)(void*)"../main.lua", 12);
}
