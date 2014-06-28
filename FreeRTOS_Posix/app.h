#ifndef _APP_H
#define _APP_H

/* define as extensões a serem utilizadas na aplicação */

#define EXT_TIMER
#define EXT_LUA

enum {
    EVT_TEST = EVT,     /* pula os eventos já definidos em "events.h" */

    /* cada extensão tem um conjunto de eventos */

#ifdef EXT_LUA
    EVT_LUA_OUT_FILE,
    EVT_LUA_OUT_STRING,
#endif

#ifdef EXT_TIMER
    EVT_TIMER_OUT_START,
    EVT_TIMER_IN_BUSY,
    EVT_TIMER_IN_EXPIRED,
#endif
};

#endif
