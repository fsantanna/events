#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "events.h"

/* event_t
 * Represents an occuring event.
 */

        /* event id (or type)
         * e.g. EVT_TIMER, EVT_LUA, EVT_GPIO
         */
typedef struct {
    event_id_t    id;
    event_param_t param;
    int16_t       sz;   /* signed because of fill */
    char          buf[0];
} event_t;

#define QUEUE_MAX 256

char    QUEUE[QUEUE_MAX];
int     QUEUE_tot = 0;
uint8_t QUEUE_get = 0;      /* u8 because of 256 max */
uint8_t QUEUE_put = 0;

event_t* queue_get (void) {
    event_t* ret;
    //CEU_ISR_OFF();
    if (QUEUE_tot == 0) {
        ret = NULL;
    } else {
#ifdef DEBUG
        assert(QUEUE_tot > 0);
#endif
        ret = (event_t*) &QUEUE[QUEUE_get];
    }
    //CEU_ISR_ON();
    return ret;
}

/* TODO: retorno "int" assume que o usuario vai ter o que fazer, caso 0 */
int queue_put (event_id_t id, event_param_t param,
               int sz, char* buf) {
    //CEU_ISR_OFF();

    int n = sizeof(event_t) + sz;

    if (QUEUE_tot+n > QUEUE_MAX)
        return 0;   /* TODO: add event FULL when QUEUE_MAX-1 */

    /* An event+data must be continuous in the QUEUE. */
    if (QUEUE_put+n+sizeof(event_t)>=QUEUE_MAX && id!=EVT_NONE) {
        int fill = QUEUE_MAX - QUEUE_put - sizeof(event_t);
        /*_ceu_sys_emit(app, EVT_NONE, param, fill, NULL);*/
        event_t* evt = (event_t*) &QUEUE[QUEUE_put];
        evt->id = EVT_NONE;
        evt->sz  = fill;
        QUEUE_put += sizeof(event_t) + fill;
        QUEUE_tot += sizeof(event_t) + fill;
    }

    {
        event_t* evt = (event_t*) &QUEUE[QUEUE_put];
        evt->id = id;
        evt->sz  = sz;

        if (sz == 0) {
            /* "param" is self-contained */
            evt->param = param;
        } else {
            /* "param" points to "buf" */
            evt->param.ptr = evt->buf;
            memcpy(evt->buf, buf, sz);
        }
    }
    QUEUE_put += n;
    QUEUE_tot += n;

    //CEU_ISR_ON();
    return 1;
}

void queue_rem (void) {
    //CEU_ISR_OFF();
    event_t* evt = (event_t*) &QUEUE[QUEUE_get];
    QUEUE_tot -= sizeof(event_t) + evt->sz;
    QUEUE_get += sizeof(event_t) + evt->sz;
    //CEU_ISR_ON();
}

typedef struct listener_t {
    event_id_t         id;
    callback_t         cb;
    struct listener_t* prv;
    struct listener_t* nxt;
} listener_t;

listener_t* LISTENERS = NULL;

/* ponto de falha! */
void listener_add (event_id_t id, callback_t cb) {
    listener_t* l = (listener_t*) malloc(sizeof(listener_t));
    assert(l != NULL);
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

void listener_rem (listener_t* l) {
    if (LISTENERS == l) {
        LISTENERS = NULL;
    } else {
        l->prv->nxt = l->nxt;
    }
    free(l);
}

void scheduler (void)
{
    while (1)
    {
        /* clear the current size (ignore events emitted here) */
        //CEU_ISR_OFF();
        int has = QUEUE_tot;
        //CEU_ISR_ON();
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
    }
}


