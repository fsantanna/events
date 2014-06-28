#ifndef _EVENTS_H
#define _EVENTS_H

#define DEBUG

#include <stdint.h>

/*
 * Eventos pre-definidos.
 * Os eventos das extensões vão de `EVT` em diante (veja app.h).
 */

enum {
    EVT_NONE = 0,
    EVT_QUIT,
    EVT,
};

/*
 * Eventos de "panic".
 * Não vão pela fila, em vez disso, chamam a função de pânico setada em 
 * `evt_init´.
 */

enum {
    EVT_ERR_LISTENER_ADD = 1,
    EVT_ERR_QUEUE_PUT,
};

/* Tipo para os identificadores de eventos. */

typedef uint8_t evt_id_t;
         /* at most 255 different event types (0 is reserved) */

/*
 * Tipo carregado por um evento.
 * Um inteiro ou um ponteiro para um tipo definido externamente.
 * (Faz diferença ao inserir um evento na fila.)
 */

typedef union {
    int   v;
    void* ptr;
} evt_param_t;

/*
 * Insere um evento na fila.
 *      - id:    identificador do event
 *      - param: payload do evento
 *      - sz:    0, caso payload seja "int"; tamanho do payload, caso seja ponteiro
 * Note que o payload do evento é copiado inteiramente para a fila (i.e. quem 
 * gera o evento não precisa manter o payload em memória).
 * Função é thread safe.
 */

void evt_queue_put (evt_id_t id, evt_param_t param, int sz);

/* Tipo para callbacks de evento (uma função que recebe o evento ocorrendo como parâmetro).  */

typedef void (*evt_cb_t) (evt_param_t);

/*
 * Adiciona e remove um listener de eventos.
 *  - add: sempre que `id` ocorrer, `cb` é chamada
 *  - rem: `cb` não mais é executada quando `id` ocorrer
 */

void evt_listener_add (evt_id_t id, evt_cb_t cb);
void evt_listener_rem (evt_id_t id, evt_cb_t cb);

/* Tipo da função de pânico. */

typedef void (*evt_panic_t) (int);

/*
 * Inicializa a biblioteca de eventos.
 *  - cb: callback de pânico
 */

void evt_init (evt_panic_t);

/*
 * Executa o scheduler (dispatcher) com o loop de eventos.
 * Termina quando um evento do tipo `EVT_QUIT` é gerado.
 */

void evt_scheduler (void);

#endif
