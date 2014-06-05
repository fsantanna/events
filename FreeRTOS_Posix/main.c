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
    E_scheduler();
    vTaskEndScheduler();
}

int main( void )
{
    void* xHandle = NULL;
    app_init();
    xTaskCreate(events_task, "EVENTS", 100, NULL, tskIDLE_PRIORITY, &xHandle);
    assert(xHandle != NULL);
    vTaskStartScheduler();
    assert(app_ret == 15);  /* apenas para testes */
    printf("OK\n");
    return 0;
}
