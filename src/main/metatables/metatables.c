#include <string.h>

#include <git2.h>
#include <lauxlib.h>

#include "metatables.h"
#include "index.h"
#include "gc.h"
#include "tostring.h"
#include "len.h"

/**
 * 给所有 libgit2 的对象创建 metatable，以及注册 gc 函数
 */

#define SETFUNCS(lua_state, metatable, funcs) \
    luaL_newmetatable(lua_state, metatable); \
    luaL_setfuncs(lua_state, funcs, 0);

void lmetatables_init(lua_State *l) {
    luaL_Reg end = {NULL, NULL};

    SETFUNCS(l, LUA_METATABLE_GIT_REPOSITORY, ((luaL_Reg []) {
        {"__gc", lm_gc_repository}, end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_INDEX, ((luaL_Reg []) {
        {"__gc", lm_gc_index}, end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_TREE, ((luaL_Reg []) {
        {"__gc", lm_gc_tree}, end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_OBJECT, ((luaL_Reg[]) {
        {"__gc", lm_gc_object}, end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_REFERENCE, ((luaL_Reg []) {
        {"__gc", lm_gc_reference}, end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_REMOTE, ((luaL_Reg []) {
        {"__gc", lm_gc_remote}, end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_COMMIT, ((luaL_Reg []) {
        {"__gc", lm_gc_commit}, end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_SIGNATURE, ((luaL_Reg []) {
        {"__gc", lm_gc_signature},
        {"__index", lm_index_signature},
        {"__tostring", lm_tostring_signature},
        end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_TIME, ((luaL_Reg []) {
        {"__index", lm_index_time},
        {"__tostring", lm_tostring_time},
        end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_STATUS_LIST, ((luaL_Reg []) {
        {"__gc", lm_gc_status_list},
        {"__len", lm_len_status_list},
        {"__index", lm_index_status_list},
        end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_STATUS_ENTRY, ((luaL_Reg []) {
        {"__index", lm_index_status_entry},
        {"__tostring", lm_tostring_status_entry},
        end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_DIFF_DELTA, ((luaL_Reg []) {
        {"__tostring", lm_tostring_diff_delta},
        end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_DIFF_FILE, ((luaL_Reg []) {
        {"__tostring", lm_tostring_diff_file},
        end
    }));
    SETFUNCS(l, LUA_METATABLE_GIT_STRARRAY, ((luaL_Reg []) {
        {"__gc", lm_gc_strarray},
        {"__index", lm_index_strarray},
        {"__len", lm_len_strarray},
        {"__tostring", lm_tostring_strarray},
        end
    }));

    luaL_newmetatable(l, LUA_METATABLE_GIT_OID);
    luaL_newmetatable(l, LUA_METATABLE_GIT_PUSH_OPTIONS);
}
