#include "app.h"
#include "ext_timer.h"
#include "ext_lua.h"

int app_ret = 1;

void panic (int err) {
    printf("ERR = %d\n", err);
    app_ret *= 2;
}

void cb_2 (E_event_param_t param) {
    printf("cb_2 = %d\n", param.v);
    app_ret += 2*param.v;
}

void cb_1 (E_event_param_t param) {
    printf("cb_1 = %d\n", param.v);
    app_ret += param.v;
    if (param.v == 2) {
        E_listener_rem(0, cb_1);
        E_listener_add(2, cb_2);
    }
}

void cb_timer (E_event_param_t param) {
    //printf("cb_timer\n");
    //E_queue_put(E_EVT_QUIT, (E_event_param_t)0, 0);
}

void app_init (void) {
    E_init(panic);

#ifdef EXT_TIMER
    ext_timer_init();
#endif
#ifdef EXT_LUA
    ext_lua_init();
#endif

    E_listener_add(E_EVT_TEST, cb_1);
    E_listener_add(E_EVT_TIMER_IN_EXPIRED, cb_timer);

    E_queue_put(E_EVT_TEST, (E_event_param_t)NULL, 100000);

    E_queue_put(E_EVT_TEST, (E_event_param_t)1, 0);
    E_queue_put(E_EVT_TEST, (E_event_param_t)2, 0);
    E_queue_put(E_EVT_TEST, (E_event_param_t)3, 0);

    E_queue_put(E_EVT_TIMER_OUT_START, (E_event_param_t)2000, 0);

    E_queue_put(E_EVT_TIMER_OUT_START, (E_event_param_t)2000, 0);

    E_queue_put(E_EVT_LUA_OUT_FILE, (E_event_param_t)(void*)"../main.lua", 12);
}
