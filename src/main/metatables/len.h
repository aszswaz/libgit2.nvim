#include "git2.h"
#include <lua.h>

/**
 * 获取 git_status_list 的大小
 */
static int lm_len_status_list(lua_State *l) {
    git_status_list *list = *(git_status_list **)lua_touserdata(l, 1);
    lua_pushinteger(l, git_status_list_entrycount(list));
    return 1;
}

static int lm_len_strarray(lua_State *l) {
    git_strarray *array = *(git_strarray **)lua_touserdata(l, 1);
    lua_pushnumber(l, array->count);
    return 1;
}
