#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "lua/src/lua.h"
#include "lua/src/lauxlib.h"
#include "lua/src/lualib.h"

#include "events.h"
#include "app.h"
#include "ext_lua.h"

static lua_State* L = NULL;

static void cb_file (evt_param_t param) {
    int ok = luaL_dofile(L, (char*)param.ptr);
    // TODO: ok (generate ERR with error message)
}

static int l_post (lua_State* L) {
    // TODO: check all lua returns
    const char* evt = lua_tostring(L,-2);
    if (!strncmp(evt, "EVT_TIMER_OUT_START", 30)) {
        int timer = lua_tonumber(L,-1);
        evt_queue_put(EVT_TIMER_OUT_START, (evt_param_t)timer, 0);
    } else {
        // TODO
    }
    return 0;
}

/* Para cada evento exportado para Lua, ouvi-lo e mapea-lo para 
 * "events.dispatch()" */
static void cb_e_evt_timer_in_expired (evt_param_t param) {
    lua_getglobal(L, "events");         // [ events ]
    lua_getfield(L, -1, "dispatch");    // [ events | dispatch ]
    lua_pushstring(L, "EVT_TIMER_IN_EXPIRED");    // [ ...   | id ]
    lua_call(L, 1, 0);                  // [ events ]
    lua_pop(L, 1);                      // [ ]
        // TODO: testar retorno
}

void ext_lua_init (void) {
    L = luaL_newstate();
    assert(L != NULL);
    luaL_openlibs(L);
    assert(luaL_dofile(L,"../events.lua") == 0);

    lua_getglobal(L, "events");     // [ events ]
    lua_pushcfunction(L, l_post);   // [ events | post ]
    lua_setfield(L, -2, "post");    // [ events ]
    lua_pop(L, 1);                  // [ ]

    evt_listener_add(EVT_LUA_OUT_FILE, cb_file);

    evt_listener_add(EVT_TIMER_IN_EXPIRED, cb_e_evt_timer_in_expired);
}
