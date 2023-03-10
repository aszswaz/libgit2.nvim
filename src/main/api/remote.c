#include "api.h"
#include "../util/allocator.h"
#include "../logger.h"

/**
 * 创建远程仓库
 *
 * @param repo 仓库句柄
 * @param name 远程仓库的名称
 * @param url 远程仓库的地址
 * @return 远程仓库的句柄
 */
static int lremote_create(lua_State *l) {
    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);
    const char *name = luaL_checkstring(l, 2);
    const char *url = luaL_checkstring(l, 3);

    git_remote **remote = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_REMOTE);
    LUA_GIT_ERROR(l, git_remote_create(remote, repo, name, url));
    return 1;
}

/**
 * 获取远程仓库列表
 *
 * @param repo 仓库的句柄
 * @return 远程仓库的列表
 */
static int lremote_list(lua_State *l) {
    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);

    git_strarray **array = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_STRARRAY);
    *array = LMALLOC(sizeof(git_strarray));
    memset(*array, 0, sizeof(git_strarray));
    LUA_GIT_ERROR(l, git_remote_list(*array, repo));
    return 1;
}

/**
 * 根据远程仓库的名称获取远程仓库的句柄
 *
 * @param repo 存储库的句柄
 * @param name 远程仓库的名称
 * @return 远程仓库的句柄
 */
static int lremote_lookup(lua_State *l) {
    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);
    const char *name = luaL_checkstring(l, 2);

    git_remote **remote = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_REMOTE);
    *remote = NULL;
    LUA_GIT_ERROR(l, git_remote_lookup(remote, repo, name));
    return 1;
}

/**
 * 获取远程仓库的地址
 *
 * @param remote 远程仓库的句柄
 * @return 远程仓库的地址
 */
static int lremote_url(lua_State *l) {
    git_remote *remote = *(git_remote **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REMOTE);
    const char *url = git_remote_url(remote);
    lua_pushstring(l, url);
    return 1;
}

/**
 * 获取远程仓库的推送地址
 *
 * @param remote 远程仓库的句柄
 * @return 远程仓库的推送地址
 */
static int lremote_pushurl(lua_State *l) {
    git_remote *remote = *(git_remote **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REMOTE);
    const char *url = git_remote_pushurl(remote);
    lua_pushstring(l, url);
    return 1;
}

/**
 * 删除远程仓库
 *
 * @param repo 存储库的句柄
 * @param name 远程仓库的名字
 */
static int lremote_delete(lua_State *l) {
    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);
    const char *name = luaL_checkstring(l, 2);
    LUA_GIT_ERROR(l, git_remote_delete(repo, name));
    return 0;
}

/**
 * 重命名远程仓库
 *
 * @param repo     存储库
 * @param name     原名
 * @param new_name 新的名称
 */
static int lremote_rename(lua_State *l) {

    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);
    const char *name = luaL_checkstring(l, 2);
    const char *new_name = luaL_checkstring(l, 3);

    git_strarray **array = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_STRARRAY);
    *array = LMALLOC(sizeof(git_strarray));
    memset(*array, 0, sizeof(git_strarray));
    LUA_GIT_ERROR(l, git_remote_rename(*array, repo, name, new_name));
    return 1;
}

/**
 * 设置远程仓库的 url
 *
 * @param repo   存储库
 * @param remote 远程仓库名称
 * @param url    远程仓库地址
 * @param push   只设置要推送的远程仓库地址
 */
static int lremote_seturl(lua_State *l) {
    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);
    const char *remote = luaL_checkstring(l, 2);
    const char *url = luaL_checkstring(l, 3);
    int is_pushurl = lua_toboolean(l, 4);

    if (is_pushurl) {
        LUA_GIT_ERROR(l, git_remote_set_url(repo, remote, url));
    } else {
        LUA_GIT_ERROR(l, git_remote_set_pushurl(repo, remote, url));
    }
    return 0;
}

/**
 * 推送到远程仓库
 *
 * @param remote   远程仓库的句柄
 * @param refspecs 存储分支的引用
 * @param opts     推送选项
 */
static int lremote_push(lua_State *l) {
    git_remote *remote = *(git_remote **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REMOTE);
    git_push_options *opts = luaL_checkudata(l, 3, LUA_METATABLE_GIT_PUSH_OPTIONS);

    // 处理参数 refspecs
    git_strarray *refspecs = NULL;
    if (lua_isstring(l, 2)) {
        refspecs = str2git_strarray(l, 2);
    } else if (lua_istable(l, 2)) {
        refspecs = table2git_strarray(l, 2);
    } else {
        luaL_error(l,
                   "bad argument #2 to 'push' (expected string or table, got nil)",
                   luaL_typename(l, 2)
                  );
    }

    LUA_GIT_ERROR(l, git_remote_push(remote, refspecs, opts));
    return 0;
}

void lremote_minit(lua_State *l, int module_table) {
    luaL_Reg funcs01[] = {
        {"create", lremote_create},
        {"list", lremote_list},
        {"lookup", lremote_lookup},
        {"url", lremote_url},
        {"pushurl", lremote_pushurl},
        {"delete", lremote_delete},
        {"rename", lremote_rename},
        {"seturl", lremote_seturl},
        {"push", lremote_push},
        {NULL, NULL}
    };
    luaL_newlib(l, funcs01);
    lua_setfield(l, module_table, "remote");
}
