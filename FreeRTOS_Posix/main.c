// TODO: ext_*_close()

void vMainQueueSendPassed(void) {}
void vApplicationIdleHook(void) {}

#include <assert.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "events.h"
#include "app.c"

void events_task (void* pvParameters) {
    evt_scheduler();
    vTaskEndScheduler();
}

#define BUFFER_LENGTH 256

int main( void )
{
/*
    char  line[BUFFER_LENGTH];
    char* p;

    for (;;) {
        printf("PROMPT > ");
        p = fgets(line, BUFFER_LENGTH, stdin);
        printf("ERR %d %d\n", ferror(stdin), feof(stdin));
        if (p == NULL) continue;
        printf("OI %s\n", line);
    }
*/

    void* xHandle = NULL;
    app_init();
    xTaskCreate(events_task, (const signed char * const)"EVENTS", 100, NULL, tskIDLE_PRIORITY, &xHandle);
    assert(xHandle != NULL);
    vTaskStartScheduler();
    assert(app_ret == 15);  /* apenas para testes */
    printf("OK\n");
    return 0;
}
