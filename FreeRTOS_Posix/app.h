#ifndef _APP_H
#define _APP_H

#define EXT_TIMER
#define EXT_LUA

enum {
    E_EVT_TEST = E_EVT,

#ifdef EXT_TIMER
    E_EVT_TIMER_OUT_START,
    E_EVT_TIMER_IN_BUSY,
    E_EVT_TIMER_IN_EXPIRED,
#endif

#ifdef EXT_LUA
    E_EVT_LUA_OUT_FILE,
    E_EVT_LUA_OUT_STRING,
#endif
};

#endif
