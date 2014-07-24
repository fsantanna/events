#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "FreeRTOS.h"
#include "task.h"

#include "events.h"
#include "app.h"
#include "ext_lua.h"
#include "ext_console.h"

#define BUFFER_LENGTH 512

char buffer[BUFFER_LENGTH];

static int incomplete (const char* msg) {
    //if (status == LUA_ERRSYNTAX) {
        size_t lmsg = strlen(msg);
        const char *tp = msg + lmsg - (sizeof("<eof>") - 1);
        if (strstr(msg, "<eof>") == tp) {
            return 1;
        }
    //}
    return 0;  /* else... */
}

void cb_lua_ack (evt_param_t param) {
    evt_lua_in_ack_t* ret = (evt_lua_in_ack_t*) param.ptr;
    if (ret->err) {
        if (incomplete(ret->msg)) {
            // incomplete command
        } else {
            printf("LUA ERROR: %s\n", ret->msg);
            buffer[0] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

void console_task (void* pvParameters) {
    buffer[0] = '\0';

    strcat(buffer, "if then");
printf("TRY: %s\n", buffer);
    evt_queue_put(EVT_LUA_OUT_STRING, (evt_param_t)(void*)buffer,
                                      strlen(buffer)+1);
    vTaskDelay(1000);

    strcat(buffer, "a=");
printf("TRY: %s\n", buffer);
    evt_queue_put(EVT_LUA_OUT_STRING, (evt_param_t)(void*)buffer,
                                      strlen(buffer)+1);
    vTaskDelay(1000);

    strcat(buffer, "1");
printf("TRY: %s\n", buffer);
    evt_queue_put(EVT_LUA_OUT_STRING, (evt_param_t)(void*)buffer,
                                      strlen(buffer)+1);
    vTaskDelay(1000);

    strcat(buffer, "print(a)");
printf("TRY: %s\n", buffer);
    evt_queue_put(EVT_LUA_OUT_STRING, (evt_param_t)(void*)buffer,
                                      strlen(buffer)+1);
    vTaskDelay(1000);

/*
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
*/
}

void ext_console_init (void) {
    void* xHandle = NULL;
    xTaskCreate(console_task, (const signed char * const)"CONSOLE", 100, NULL, tskIDLE_PRIORITY, &xHandle);
    assert(xHandle != NULL);

    evt_listener_add(EVT_LUA_IN_ACK, cb_lua_ack);
}
