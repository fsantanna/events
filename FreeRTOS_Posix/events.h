#ifndef _EVENTS_H
#define _EVENTS_H

#include <stdint.h>

enum {
    EVT_NONE = 0
};

typedef uint8_t event_id_t;
         /* at most 255 (0 is reserved) different event types */

typedef union {
    int   v;
    void* ptr;
} event_param_t;

int queue_put (event_id_t id, event_param_t param, int sz, char* buf);

typedef void (*callback_t) (event_param_t);

void listener_add (event_id_t id, callback_t cb);

void scheduler (void);

#endif
