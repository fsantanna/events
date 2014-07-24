#ifndef _LUA_H
#define _LUA_H

#define EVT_LUA_IN_ACK_MSG_MAX 128

typedef struct {
    char err;
    char msg[EVT_LUA_IN_ACK_MSG_MAX];
} evt_lua_in_ack_t;

void ext_lua_init (void);

#endif
