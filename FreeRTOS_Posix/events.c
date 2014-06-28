#include "events.h"

#ifdef DEBUG
#include <assert.h> // TODO
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "semphr.h"

static xSemaphoreHandle MUTEX = NULL;
static evt_panic_t      PANIC = NULL;

/* event_t
 * Represents an occuring event.
 */

        /* event id (or type)
         * e.g. EVT_TIMER, EVT_LUA, EVT_GPIO
         */
typedef struct {
    evt_id_t    id;
    evt_param_t param;
    int16_t     sz;   /* signed because of fill */
    char        buf[0];
} event_t;

#define QUEUE_MAX 256

static char    QUEUE[QUEUE_MAX];
static int     QUEUE_tot = 0;
static uint8_t QUEUE_get = 0;      /* u8 because of 256 max */
static uint8_t QUEUE_put = 0;

static event_t* queue_get (void) {
    event_t* ret;
    xSemaphoreTake(MUTEX, portMAX_DELAY);
    if (QUEUE_tot == 0) {
        ret = NULL;
    } else {
#ifdef DEBUG
        assert(QUEUE_tot > 0);
#endif
        ret = (event_t*) &QUEUE[QUEUE_get];
    }
    xSemaphoreGive(MUTEX);
    return ret;
}

void evt_queue_put (evt_id_t id, evt_param_t param, int sz) {
    xSemaphoreTake(MUTEX, portMAX_DELAY);

    int n = sizeof(event_t) + sz;

    if (QUEUE_tot+n > QUEUE_MAX) {
        xSemaphoreGive(MUTEX);
        PANIC(EVT_ERR_QUEUE_PUT);
        return;
    }

    /* An event+data must be continuous in the QUEUE. */
    if (QUEUE_put+n+sizeof(event_t)>=QUEUE_MAX && id!=EVT_NONE) {
        int fill = QUEUE_MAX - QUEUE_put - sizeof(event_t);
        /*_ceu_sys_emit(app, EVT_NONE, param, fill, NULL);*/
        event_t* evt = (event_t*) &QUEUE[QUEUE_put];
        evt->id = EVT_NONE;
        evt->sz = fill;
        QUEUE_put += sizeof(event_t) + fill;
        QUEUE_tot += sizeof(event_t) + fill;
    }

    {
        event_t* evt = (event_t*) &QUEUE[QUEUE_put];
        evt->id = id;
        evt->sz  = sz;

        if (sz == 0) {
            /* "param" is self-contained */
            evt->param.v = param.v;
        } else {
            /* "param" points to "buf" */
            memcpy(evt->buf, param.ptr, sz);
            evt->param.ptr = evt->buf;
        }
    }
    QUEUE_put += n;
    QUEUE_tot += n;

    xSemaphoreGive(MUTEX);
}

static void queue_rem (void) {
    xSemaphoreTake(MUTEX, portMAX_DELAY);
    event_t* evt = (event_t*) &QUEUE[QUEUE_get];
    QUEUE_tot -= sizeof(event_t) + evt->sz;
    QUEUE_get += sizeof(event_t) + evt->sz;
    xSemaphoreGive(MUTEX);
}

typedef struct listener_t {
    evt_id_t       id;
    evt_cb_t       cb;
    struct listener_t* prv;
    struct listener_t* nxt;
} listener_t;

static listener_t* LISTENERS = NULL;

/* ponto de falha! */
void evt_listener_add (evt_id_t id, evt_cb_t cb) {
    listener_t* l = (listener_t*) malloc(sizeof(listener_t));
    if (l == NULL) {
        PANIC(EVT_ERR_LISTENER_ADD);
    }

    if (LISTENERS == NULL) {
        LISTENERS = l;
    } else {
        l->prv = LISTENERS->prv;    /* previous last listener */
        LISTENERS->prv->nxt = l;    /* now points to "l" */
    }
    LISTENERS->prv = l;             /* "l" is the new last */
    l->nxt = NULL;
    l->id  = id;
    l->cb  = cb;
}

static void listener_rem (listener_t* l) {
    if (LISTENERS == l) {
        LISTENERS = NULL;
    } else {
        l->prv->nxt = l->nxt;
    }
    free(l);
}

void evt_listener_rem (evt_id_t id, evt_cb_t cb) {
    listener_t* l = LISTENERS;
    while (l != NULL) {
        if ((l->id==id || id==EVT_NONE) && (l->cb==NULL || l->cb==cb)) {
            listener_rem(l);
        }
        l = l->nxt;
    }
}

void evt_init (evt_panic_t cb) {
    MUTEX = xSemaphoreCreateMutex();
    assert(MUTEX != NULL);
    PANIC = cb;
}

void evt_scheduler (void)
{
    while (1)
    {
        xSemaphoreTake(MUTEX, portMAX_DELAY);
        int has = QUEUE_tot;
        xSemaphoreGive(MUTEX);
        if (! has) {
            // TODO: conditional variable?
            continue;
        }

        event_t* evt = queue_get();
        if (evt->id == EVT_NONE) {
            /* nothing; */
            /* "fill event" */

        } else {
            listener_t* l = LISTENERS;
            while (l != NULL) {
                if (l->id == evt->id) {
                    l->cb(evt->param);
                }
                l = l->nxt;
            }
        }

        queue_rem();

        if (evt->id == EVT_QUIT) {
            break;
        }
    }
}


