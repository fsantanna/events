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

#ifdef EXT_SEND
#include "ext_send.h"
#endif

static lua_State* L = NULL;

static void cb_file_string (int err) {
    evt_lua_in_ack_t ret;
    ret.err = err;
    if (err != 0) {                     // [ ... | msg ]
        const char* msg = lua_tostring(L, -1);
        strncpy(ret.msg, msg, EVT_LUA_IN_ACK_MSG_MAX);
        lua_pop(L, 1);                  // [ ... ]
    }
    evt_queue_put(EVT_LUA_IN_ACK, (evt_param_t)(void*)&ret,
                                  sizeof(evt_lua_in_ack_t));
}

static void cb_file (evt_param_t param) {
    int err = luaL_dofile(L, (char*)param.ptr);     // [ ... | str? ]
    cb_file_string(err);
}

static void cb_string (evt_param_t param) {
    int err = luaL_dostring(L, (char*)param.ptr);
    cb_file_string(err);
}

static int l_post (lua_State* L) {
    // TODO: check all lua returns
    const const char* evt = lua_tostring(L,1);
    if (!strncmp(evt, "EVT_LUA_OUT_STRING", 30)) {
        const char* str = lua_tostring(L,2);
        evt_queue_put(EVT_LUA_OUT_STRING, (evt_param_t)(void*)str, strlen(str));
#ifdef EXT_TIMER
    } else if (!strncmp(evt, "EVT_TIMER_OUT_START", 30)) {
        int timer = lua_tonumber(L,-1);
        evt_queue_put(EVT_TIMER_OUT_START, (evt_param_t)timer, 0);
    } else if (!strncmp(evt, "EVT_TIMER_OUT_STOP", 30)) {
        evt_queue_put(EVT_TIMER_OUT_STOP, (evt_param_t)0, 0);
#endif
#ifdef EXT_SEND
    } else if (!strncmp(evt, "EVT_SEND_OUT_START", 30)) {
        size_t len;
        const char* val = luaL_checklstring(L, 2, &len);
        evt_queue_put(EVT_SEND_OUT_START, (evt_param_t)(void*)val, len+1);
    } else if (!strncmp(evt, "EVT_SEND_OUT_STOP", 30)) {
        evt_queue_put(EVT_SEND_OUT_STOP, (evt_param_t)0, 0);
#endif
#ifdef EXT_BUTTON
    } else if (!strncmp(evt, "EVT_BUTTON_IN_PRESSED", 30)) {
        evt_queue_put(EVT_BUTTON_IN_PRESSED, (evt_param_t)0, 0);
#endif
    } else {
        evt_queue_put(EVT_LUA_IN_ERR, (evt_param_t)(void*)"unknown event",
                                      strlen("unknown event")+1);
    }
    return 0;
}

/* Para cada evento exportado para Lua, ouvi-lo e mapea-lo para 
 * "events.dispatch()" */

void cb_call (int n) {
    int err = lua_pcall(L, n, 0, 0);    // [ events ]
    if (err != 0) {                     // [ events | err_msg ]
        const char* err_msg = lua_tostring(L, -1);
        evt_queue_put(EVT_LUA_IN_ERR, (evt_param_t)(void*)err_msg,
                                      strlen(err_msg)+1);
        lua_pop(L, 1);                  // [ events ]
    }
    lua_pop(L, 1);                      // [ ]
}

#ifdef EXT_TIMER
static void cb_e_evt_timer_in_expired (evt_param_t param) {
    lua_getglobal(L, "events");         // [ events ]
    lua_getfield(L, -1, "dispatch");    // [ events | dispatch ]
    lua_pushstring(L, "EVT_TIMER_IN_EXPIRED"); // [ ...   | id ]
    cb_call(1);                         // [ ]
}
#endif

#ifdef EXT_SEND
static void cb_e_evt_send_in_ack (evt_param_t param) {
    lua_getglobal(L, "events");         // [ events ]
    lua_getfield(L, -1, "dispatch");    // [ events | dispatch ]
    lua_pushstring(L, "EVT_SEND_IN_ACK"); // [ ...   | id ]
    lua_pushnumber(L, param.v);         // [ ... | id | len ]
    cb_call(2);                         // [ ]
}
#endif

#ifdef EXT_BUTTON
static void cb_e_evt_button_in_pressed (evt_param_t param) {
    lua_getglobal(L, "events");         // [ events ]
    lua_getfield(L, -1, "dispatch");    // [ events | dispatch ]
    lua_pushstring(L, "EVT_BUTTON_IN_PRESSED"); // [ ...   | id ]
    cb_call(1);                         // [ ]
}
#endif

void ext_lua_init (void) {
    L = luaL_newstate();
    assert(L != NULL);
    luaL_openlibs(L);
    assert(luaL_dofile(L,"../events.lua") == 0);

    lua_getglobal(L, "events");     // [ events ]
    lua_pushcfunction(L, l_post);   // [ events | post ]
    lua_setfield(L, -2, "post");    // [ events ]
    lua_pop(L, 1);                  // [ ]

    evt_listener_add(EVT_LUA_OUT_FILE,   cb_file);
    evt_listener_add(EVT_LUA_OUT_STRING, cb_string);

#ifdef EXT_TIMER
    evt_listener_add(EVT_TIMER_IN_EXPIRED,  cb_e_evt_timer_in_expired);
#endif
#ifdef EXT_SEND
    evt_listener_add(EVT_SEND_IN_ACK, cb_e_evt_send_in_ack);
#endif
#ifdef EXT_BUTTON
    evt_listener_add(EVT_BUTTON_IN_PRESSED, cb_e_evt_button_in_pressed);
#endif
}
