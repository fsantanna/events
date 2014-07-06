#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "events.h"
#include "app.h"
#include "ext_send.h"

int send = 0;
char str[256];

void send_start_cb (evt_param_t param) {
    if (send) {
        evt_queue_put(EVT_SEND_IN_BUSY, (evt_param_t)0, 0);
        return;
    }
    strncpy(str, (char*)param.ptr, 255);
    send = 1;
}

void send_stop_cb (evt_param_t param) {
    // TODO: incompleto (deveria interromper)
    send = -1;
}

void send_task (void* pvParameters) {
    for (;;) {
        vTaskDelay(10);
        if (send == 1) {
            int len = strlen(str);
            vTaskDelay(200);
            if (send == 1) {
                evt_queue_put(EVT_SEND_IN_ACK, (evt_param_t)len, 0);
            }
            send = 0;
        }
    }
}

void ext_send_init (void) {
    void* xHandle = NULL;
    evt_listener_add(EVT_SEND_OUT_START, send_start_cb);
    evt_listener_add(EVT_SEND_OUT_STOP,  send_stop_cb);
    xTaskCreate(send_task, (const signed char * const)"SEND", 100, NULL, tskIDLE_PRIORITY, &xHandle);
    assert(xHandle != NULL);
}
