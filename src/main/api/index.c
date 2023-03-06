#include <stdio.h>

#include <git2.h>
#include <lua.h>
#include <lauxlib.h>

#include "../error.h"
#include "../metatables/metatables.h"
#include "../util/util.h"

/**
 * 将存储库索引的更改保存到磁盘
 */
static int lindex_write(lua_State *l) {
    git_index *index = *(git_index **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_INDEX);
    LUA_GIT_ERROR(l, git_index_write(index));
    return 0;
}

/**
 * 此方法将扫描索引并将其当前状态的表示写回磁盘； 它递归地为存储在索引中的每个子树创建树对象，但只返回根树的 OID。 这是可以使用的 OID，例如 创建一个提交。
 * 索引实例不能是裸的，需要关联到现有的存储库。
 * 索引不得包含任何冲突文件。
 *
 * @return 根树的 ID
 */
static int lindex_write_tree(lua_State *l) {
    git_index *index = *(git_index **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_INDEX);

    git_oid *oid = new_userdata(l, sizeof(git_oid), LUA_METATABLE_GIT_OID);
    LUA_GIT_ERROR(l, git_index_write_tree(oid, index));
    return 1;
}

/**
 * 在存储库中添加文件的索引
 */
static int lindex_add_bypath(lua_State *l) {
    git_index *index = *(git_index **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_INDEX);
    const char *path = luaL_checkstring(l, 2);
    LUA_GIT_ERROR(l, git_index_add_bypath(index, path));
    return 0;
}

/**
 * 初始化 index 模块
 */
void lindex_minit(lua_State *l, int module_table) {

    luaL_Reg funcs01[] = {
        {"write", lindex_write},
        {"write_tree", lindex_write_tree},
        {"add_bypath", lindex_add_bypath},
        {NULL, NULL}
    };
    luaL_newlib(l, funcs01);
    lua_setfield(l, module_table, "index");
}
