#ifndef _APP_H
#define _APP_H

/* Define as extensões a serem utilizadas na aplicação: */

#define EXT_LUA
#define EXT_TIMER
#define EXT_SEND
#define EXT_CONSOLE
#define EXT_BUTTON

/*
 * Define os eventos disponíveis na aplicação:
 * EVT_<EXT>_<DIR>_<NAME>
 *      - <EXT>: nome da extensão que manipula o evento
 *      - <DIR>: direção do evento do ponto de vista da aplicação:
 *          - OUT: a aplicação emite um evento na direção da extenção
 *          - IN:  a extenção emite um evento na direção da aplicação
 *      - <NAME>: nome do evento
 */

enum {
    EVT_TEST = EVT,     /* pula os eventos já definidos em "events.h" */

    /* cada extensão tem um conjunto de eventos */

#ifdef EXT_LUA
    EVT_LUA_OUT_FILE,       /* executa um arquivo */
    EVT_LUA_OUT_STRING,     /* executa uma string */
    EVT_LUA_IN_ACK,         /* retorno de dofile/dostring */
    EVT_LUA_IN_ERR,         /* erro de execução */
#endif

#ifdef EXT_TIMER
    EVT_TIMER_OUT_START,    /* inicia o timer */
    EVT_TIMER_OUT_STOP,     /* cancela o timer */
    EVT_TIMER_IN_BUSY,      /* status de retorno do START */
    EVT_TIMER_IN_EXPIRED,   /* timer expirou após START */
#endif

#ifdef EXT_SEND
    EVT_SEND_OUT_START,     /* inicia o envio */
    EVT_SEND_OUT_STOP,      /* cancela o envio */
    EVT_SEND_IN_BUSY,       /* status de retorno do START */
    EVT_SEND_IN_ACK,        /* envio com sucesso */
#endif

#ifdef EXT_BUTTON
    EVT_BUTTON_IN_PRESSED,  /* o botão foi pressionado */
#endif
};

#endif
