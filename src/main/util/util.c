#include <lauxlib.h>

#include "util.h"
#include "../metatables/metatables.h"
#include "allocator.h"

void *new_userdata(lua_State *l, size_t sz, const char *metatable) {
    void *udat = lua_newuserdata(l, sz);
    memset(udat, 0, sz);
    luaL_setmetatable(l, metatable);
    return udat;
}

git_strarray *table2git_strarray(lua_State *l, int tbl_index) {
    luaL_checktype(l, tbl_index, LUA_TTABLE);
    size_t tbl_len = lua_objlen(l, tbl_index);
    git_strarray *array = LMALLOC(sizeof(git_strarray));
    array->count = tbl_len;
    // 申请 string 数组的内存，并初始化内存
    array->strings = LCALLOC(tbl_len, sizeof(void *));
    for (int i = 0; i < tbl_len; i++) {
        lua_pushinteger(l, i + 1);
        lua_gettable(l, tbl_index);
        array->strings[i] = LSTRDUP(luaL_checkstring(l, -1));
    }
    git_strarray **udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_STRARRAY);
    *udat = array;
    return array;
}

git_strarray *str2git_strarray(lua_State *l, int index) {
    const char *str = luaL_checkstring(l, index);

    git_strarray *array = LMALLOC(sizeof(git_strarray));
    array->count = 1;
    array->strings = LMALLOC(sizeof(void *));
    array->strings[0] = LSTRDUP(str);
    git_strarray **udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_STRARRAY);
    *udat = array;
    return array;
}

int getintfield(lua_State *l, int tbx, const char *field) {
    lua_getfield(l, tbx, field);
    if (lua_isnil(l, -1))
        return 0;
    else
        return luaL_checkint(l, -1);
}
int getboolfield(lua_State *l, int tbx, const char *field) {
    lua_getfield(l, tbx, field);
    return lua_toboolean(l, -1);
}
void *getudatfield(lua_State *l, int tbx, const char *field, const char *tname) {
    lua_getfield(l, tbx, field);
    if (lua_isnil(l, -1)) {
        return NULL;
    } else {
        return luaL_checkudata(l, -1, tname);
    }
}
const char *getstrfield(lua_State *l, int tbx, const char *field) {
    lua_getfield(l, tbx, field);
    if (lua_isnil(l, -1)) {
        return NULL;
    } else {
        return luaL_checkstring(l, -1);
    }
}
