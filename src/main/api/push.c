#include "api.h"

static int lpush_options_init(lua_State *l) {
    uint32_t version = luaL_checkinteger(l, 1);
    git_push_options *opts = new_userdata(l, sizeof(git_push_options), LUA_METATABLE_GIT_PUSH_OPTIONS);
    LUA_GIT_ERROR(l, git_push_options_init(opts, version));
    return 1;
}

void lpush_minit(lua_State *l, int module_table) {
    luaL_Reg funcs[] = {
        {"options_init", lpush_options_init},
        {NULL, NULL}
    };
    luaL_newlib(l, funcs);
    lua_setfield(l, module_table, "push");
}
