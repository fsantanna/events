// 15:30 => 17:00 (implementacao events.[hc])

#define DEBUG

void vMainQueueSendPassed(void) {}
void vApplicationIdleHook(void) {}

#include <stdio.h>
#include "events.h"
#include "events.c"

void cb_1 (event_param_t param) {
    printf("cb1 %d\n", param.v);
}

int main( void )
{
    listener_add(1, cb_1);

    queue_put(1, (event_param_t)1, 0, NULL);
    queue_put(1, (event_param_t)2, 0, NULL);

    scheduler();
}
