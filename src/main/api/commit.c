#include <string.h>

#include <git2.h>
#include <lauxlib.h>

#include "api.h"
#include "../metatables/metatables.h"
#include "../util/util.h"
#include "../error.h"

/**
 * 创建一个新的提交
 *
 * @param repo             仓库的句柄
 * @param update_ref       要更新的引用的名称
 * @param author           作者的信息
 * @param committer        提交人
 * @param message_encoding 提交信息的字符集编码
 * @param message          提交信息
 * @param tree             存储库的树
 * @param parents          父节点列表
 * @return 新提交的 git_oid
 */
static int lcommit_create(lua_State *l) {
    git_oid *commit_oid;
    const git_commit **parents = NULL;
    size_t parent_count = 0;
    int git_status = 0;

    int index = 1;
    git_repository *repo = *(git_repository **)luaL_checkudata(l, index++, LUA_METATABLE_GIT_REPOSITORY);
    const char *update_ref = luaL_checkstring(l, index++);
    const git_signature *author = *(git_signature **)luaL_checkudata(l, index++, LUA_METATABLE_GIT_SIGNATURE);
    const git_signature *committer = *(git_signature **)luaL_checkudata(l, index++, LUA_METATABLE_GIT_SIGNATURE);
    const char *message_encoding = lua_tostring(l, index++);
    const char *message = luaL_checkstring(l, index++);
    const git_tree *tree = *(git_tree **)luaL_checkudata(l, index++, LUA_METATABLE_GIT_TREE);

    // 将 parents 从 lua 数组转换为 C 数组
    luaL_checktype(l, index, LUA_TTABLE);
    parent_count = lua_objlen(l, index);
    parents = lua_newuserdata(l, parent_count * sizeof(size_t));
    memset(parents, 0, parent_count * sizeof(size_t));
    for (int i = 0; i < parent_count; i++) {
        // 列表中的元素放到栈顶
        lua_pushinteger(l, i + 1);
        lua_gettable(l, index);
        if (lua_isnil(l, -1)) continue;
        luaL_checktype(l, -1, LUA_TUSERDATA);
        git_object *obj = *(git_object **)lua_touserdata(l, -1);
        if (git_object_type(obj) == GIT_OBJECT_COMMIT) {
            parents[i] = (git_commit *)obj;
        } else {
            luaL_error(l, "bad #%d, the %dth element is not a commit object", index, i + 1);
        }
    }

    commit_oid = new_userdata(l, sizeof(git_oid), LUA_METATABLE_GIT_OID);
    git_status = git_commit_create(commit_oid, repo, update_ref, author, committer, message_encoding, message, tree, parent_count, parents);
    LUA_GIT_ERROR(l, git_status);
    return 1;
}

void lcommit_minit(lua_State *l, int module_table) {
    luaL_Reg funcs01[] = {
        {"create", lcommit_create},
        {NULL, NULL}
    };
    luaL_newlib(l, funcs01);
    lua_setfield(l, module_table, "commit");
}
