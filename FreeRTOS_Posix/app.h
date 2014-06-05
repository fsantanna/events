#ifndef _APP_H
#define _APP_H

#define EXT_TIMER

enum {
    E_EVT_TEST = E_EVT,

#ifdef EXT_TIMER
    E_EVT_TIMER_OUT_START,
    E_EVT_TIMER_IN_BUSY,
    E_EVT_TIMER_IN_EXPIRED,
#endif
};

#endif
