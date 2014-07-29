#include "app.h"

void panic (int err) {
    printf("ERR = %d\n", err);
}

void cb_1 (evt_param_t param) {
    printf("cb_1 = %d\n", param.v);
}

void app_init (void) {
    evt_init(panic);

    evt_listener_add(EVT_TEST, cb_1);
    evt_queue_put(EVT_TEST, (evt_param_t)1, 0);
    evt_queue_put(EVT_TEST, (evt_param_t)2, 0);
    evt_queue_put(EVT_TEST, (evt_param_t)NULL, 100000);
}
