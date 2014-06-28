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
void timer_start_cb (evt_param_t param) {
    if (dt != 0) {
        evt_queue_put(EVT_TIMER_IN_BUSY, (evt_param_t)0, 0);
        return;
    }

    dt = param.v;
}

void timer_task (void* pvParameters) {
    for (;;) {
        vTaskDelay(10);
        if (dt == 0) continue;// { printf("DT=%d\n",dt); };
        //while (dt == 0);// { printf("DT=%d\n",dt); };
        vTaskDelay(200*dt / 1000);
            /* vTaskDelay suspende pelo numero de "ticks".
             * Aparentemente 1s = 200ticks */
        evt_queue_put(EVT_TIMER_IN_EXPIRED, (evt_param_t)0, 0);
    }
}

void ext_timer_init (void) {
    void* xHandle = NULL;
    evt_listener_add(EVT_TIMER_OUT_START, timer_start_cb);
    xTaskCreate(timer_task, "TIMER", 100, &dt, tskIDLE_PRIORITY, &xHandle);
    assert(xHandle != NULL);
}
