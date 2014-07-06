#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "FreeRTOS.h"
#include "task.h"

#include "events.h"
#include "app.h"
#include "ext_console.h"

#define BUFFER_LENGTH 256

void console_task (void* pvParameters) {
    char  line[BUFFER_LENGTH];
    char* p;

vTaskDelay(100000);
    for (;;) {
        printf("PROMPT > ");
        p = fgets(line, BUFFER_LENGTH, stdin);
        printf("ERR %d %d %d\n", ferror(stdin), feof(stdin), EPERM);
        if (p == NULL) continue;
        //int n = scanf("%255s", line);
        //if (n == 0) continue;

        if (!strcmp(line, "BUTTON")) {
            evt_queue_put(EVT_BUTTON_IN_PRESSED, (evt_param_t)0, 0);
        }
    }
}

void ext_console_init (void) {
    void* xHandle = NULL;
    xTaskCreate(console_task, (const signed char * const)"CONSOLE", 100, NULL, tskIDLE_PRIORITY, &xHandle);
    assert(xHandle != NULL);
}
