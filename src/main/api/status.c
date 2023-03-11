#include <stdint.h>
#include <string.h>

#include "api.h"
#include "../util/allocator.h"
#include "../logger.h"

/**
 * 获取文件状态信息
 */
static int lstatus_list_new(lua_State *l) {
    git_status_options opt = {};

    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);

    if (lua_isnoneornil(l, 2)) {
        LUA_GIT_ERROR(l, git_status_options_init(&opt, GIT_STATUS_OPTIONS_VERSION));
    } else {
        table2git_status_options(l, 2, &opt);
    }

    git_status_list **list = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_STATUS_LIST);
    LUA_GIT_ERROR(l, git_status_list_new(list, repo, &opt));
    return 1;
}

/**
 * 初始化 status 模块
 */
void lstatus_minit(lua_State *l, int module) {
    lua_createtable(l, 1, 1);

    luaL_setfuncs(l, ((luaL_Reg []) {
        {"list_new", lstatus_list_new},
        {NULL, NULL}
    }), 0);

    lua_setfield(l, module, "status");
}
