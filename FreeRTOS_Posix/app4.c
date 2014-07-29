/*
 * Exemplo de uma aplicação.
 */

#include "app.h"

/* Inclui as extenções utilizadas na aplicação. */

#ifdef EXT_LUA
#include "ext_lua.h"
#endif
#ifdef EXT_TIMER
#include "ext_timer.h"
#endif

void panic (int err) {
    printf("ERR = %d\n", err);
}

#ifdef EXT_LUA
void cb_lua_err (evt_param_t param) {
    printf("LUA ERROR: %s\n", (char*)param.ptr);
}
void cb_lua_ack (evt_param_t param) {
    evt_lua_in_ack_t* ret = (evt_lua_in_ack_t*) param.ptr;
    if (ret->err) {
        printf("LUA ACK: %d %s\n", ret->err, ret->msg);
    } else {
        printf("LUA ACK: %d\n", ret->err);
    }
}
#endif

void app_init (void)
{
    /* inicializa a biblioteca de eventos com uma função de pânico */
    evt_init(panic);

    /* inicializa as extensões utilizadas */
#ifdef EXT_LUA
    ext_lua_init();
    evt_listener_add(EVT_LUA_IN_ERR, cb_lua_err);
    //evt_listener_add(EVT_LUA_IN_ACK, cb_lua_ack);
#endif
#ifdef EXT_TIMER
    ext_timer_init();
#endif

    /* Cria a aplicação, i.e., interliga as extensões que passam a se
     * comunicar através de eventos: */

    /*
     * Teste de Lua:
     * Solicita que o "driver" de Lua execute o arquivo passado.
     */
    //evt_queue_put(EVT_LUA_OUT_FILE, (evt_param_t)(void*)"../app4.lua", 12);
    //evt_queue_put(EVT_LUA_OUT_FILE, (evt_param_t)(void*)"../app5.lua", 12);
    evt_queue_put(EVT_LUA_OUT_FILE, (evt_param_t)(void*)"../app6.lua", 12);
}
