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
    if (dt > 0) {
        evt_queue_put(EVT_TIMER_IN_BUSY, (evt_param_t)0, 0);
        return;
    }
    // 1000ms = 100ticks (should be /10 not /3)
    dt = param.v/3;
}

void timer_stop_cb (evt_param_t param) {
    dt = 0;
}

void timer_task (void* pvParameters) {
    for (;;) {
        vTaskDelay(10);
        if (dt <= 0) {
            continue;       // not set
        }

        dt -= 10;
        if (dt <= 0) {
            dt = 0;
            evt_queue_put(EVT_TIMER_IN_EXPIRED, (evt_param_t)0, 0);
        }
    }
}

void ext_timer_init (void) {
    void* xHandle = NULL;
    evt_listener_add(EVT_TIMER_OUT_START, timer_start_cb);
    evt_listener_add(EVT_TIMER_OUT_STOP,  timer_stop_cb);
    xTaskCreate(timer_task, (const signed char * const)"TIMER", 100, NULL, tskIDLE_PRIORITY, &xHandle);
    assert(xHandle != NULL);
}
