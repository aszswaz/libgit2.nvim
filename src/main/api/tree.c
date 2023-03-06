#include "api.h"

/**
 * 获取存储库的树对象
 *
 * @param repo 存储库的句柄
 * @param id 树根节点的 ID
 * @return 存储库的树对象
 */
static int ltree_lookup(lua_State *l) {
    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);
    git_oid *roo_id = (git_oid *)luaL_checkudata(l, 2, LUA_METATABLE_GIT_OID);

    git_tree **tree = new_userdata(l, sizeof(size_t), LUA_METATABLE_GIT_TREE);
    LUA_GIT_ERROR(l, git_tree_lookup(tree, repo, roo_id));
    return 1;
}

void ltree_minit(lua_State *l, int module_table) {
    luaL_Reg funcs01[] = {
        {"lookup", ltree_lookup},
        {NULL, NULL}
    };
    luaL_newlib(l, funcs01);
    lua_setfield(l, module_table, "tree");
}
