#ifndef _EVENTS_H
#define _EVENTS_H

#define DEBUG

#include <stdint.h>

enum {
    E_EVT_NONE = 0,
    E_EVT_QUIT,
    E_EVT,
};

enum {
    E_ERR_LISTENER_ADD = 1,
    E_ERR_QUEUE_PUT,
};

typedef uint8_t E_event_id_t;
         /* at most 255 (0 is reserved) different event types */

typedef union {
    int   v;
    void* ptr;
} E_event_param_t;

void E_queue_put (E_event_id_t id, E_event_param_t param, int sz, char* buf);

typedef void (*E_callback_t) (E_event_param_t);

void E_listener_add (E_event_id_t id, E_callback_t cb);
void E_listener_rem (E_event_id_t id, E_callback_t cb);

typedef void (*E_panic_t) (int);
void E_init (E_panic_t);
void E_scheduler (void);

#endif
