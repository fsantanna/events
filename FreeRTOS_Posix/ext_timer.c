#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"

#include "events.h"
#include "app.h"
#include "ext_timer.h"

static int dt = 0;

#include <stdio.h>
void timer_start_cb (E_event_param_t param) {
    if (dt != 0) {
        E_queue_put(E_EVT_TIMER_IN_BUSY, (E_event_param_t)0, 0, NULL);
        return;
    }

    dt = param.v;
printf("DT %d\n", dt);
}

void timer_task (void* pvParameters) {
    for (;;) {
        vTaskDelay(100);
        if (dt == 0) continue;// { printf("DT=%d\n",dt); };
        //while (dt == 0);// { printf("DT=%d\n",dt); };
        vTaskDelay(300*dt / 1000);
            /* vTaskDelay suspende pelo numero de "ticks".
             * Aparentemente 1s = 300ticks */
        E_queue_put(E_EVT_TIMER_IN_EXPIRED, (E_event_param_t)0, 0, NULL);
    }
}

void ext_timer_init (void) {
    void* xHandle = NULL;
    E_listener_add(E_EVT_TIMER_OUT_START, timer_start_cb);
    xTaskCreate(timer_task, "TIMER", 100, &dt, tskIDLE_PRIORITY, &xHandle);
    assert(xHandle != NULL);
}
