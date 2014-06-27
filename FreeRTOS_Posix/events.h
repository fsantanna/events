#ifndef _EVENTS_H
#define _EVENTS_H

#define DEBUG

#include <stdint.h>

/*
 * Eventos pre-definidos.
 * Os eventos das extensões vão de `E_EVT` em diante (veja app.h).
 */

enum {
    E_EVT_NONE = 0,
    E_EVT_QUIT,
    E_EVT,
};

/*
 * Eventos de "panic".
 * Não vão pela fila, em vez disso, chamam a função de pânico setada em 
 * `E_init´.
 */

enum {
    E_ERR_LISTENER_ADD = 1,
    E_ERR_QUEUE_PUT,
};

/* Tipo para os identificadores de eventos. */

typedef uint8_t E_event_id_t;
         /* at most 255 different event types (0 is reserved) */

/*
 * Tipo carregado por um evento.
 * Um inteiro ou um ponteiro para um tipo definido externamente.
 * (Faz diferença ao inserir um evento na fila.)
 */

typedef union {
    int   v;
    void* ptr;
} E_event_param_t;

/*
 * Insere um evento na fila.
 *      - id:    identificador do event
 *      - param: payload do evento
 *      - sz:    no caso .
 *      - buf:   .
 * Função é thread safe.
 */

void E_queue_put (E_event_id_t id, E_event_param_t param, int sz);

typedef void (*E_callback_t) (E_event_param_t);

void E_listener_add (E_event_id_t id, E_callback_t cb);
void E_listener_rem (E_event_id_t id, E_callback_t cb);

typedef void (*E_panic_t) (int);
void E_init (E_panic_t);
void E_scheduler (void);

#endif
