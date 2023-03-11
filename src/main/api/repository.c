#include <unistd.h>
#include <stdio.h>

#include "api.h"

/**
  初始化存储库

  @path    存储库的路径
  @is_bare 是否创建裸存储库
  @return  仓库的句柄
 */
static int lrepository_init(lua_State *l) {
    git_repository **rep;
    const char *path;
    uint32_t is_bare;

    path = luaL_checkstring(l, 1);
    is_bare = lua_toboolean(l, 2);

    rep = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_REPOSITORY);
    LUA_GIT_ERROR(l, git_repository_init(rep, path, is_bare));
    // 函数会返回一个 userdata，所以函数的返回值为 1
    return 1;
}

/**
 * 打开一个已经存在的仓库
 *
 * @throw git_error
 * @return 仓库的句柄
 */
static int lrepository_open(lua_State *l) {
    const char *path = luaL_checkstring(l, 1);
    git_repository **repo = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_REPOSITORY);
    LUA_GIT_ERROR(l, git_repository_open(repo, path));
    return 1;
}

/**
 * 打开一个已经存在的仓库
 *
 * @throw git_error
 * @return 如果仓库不存在，返回 nil，如果仓库存在，返回仓库句柄
 */
static int lrepository_open_ext(lua_State *l) {
    git_repository *repo;
    int flags = 0;
    const uint8_t table_index = 2;
    const char *ceiling_dirs = NULL;

    const char *path = luaL_checkstring(l, 1);

    if (lua_istable(l, table_index)) {
        // 根据 options 设置 flags 和 ceiling_dirs
        ceiling_dirs = getstrfield(l, table_index, "ceiling_dirs");
        if (getboolfield(l, table_index, "no_search")) flags |= GIT_REPOSITORY_OPEN_NO_SEARCH;
        if (getboolfield(l, table_index, "cross_fs")) flags |= GIT_REPOSITORY_OPEN_CROSS_FS;
        if (getboolfield(l, table_index, "bare")) flags |= GIT_REPOSITORY_OPEN_BARE;
        if (getboolfield(l, table_index, "no_dotgit")) flags |= GIT_REPOSITORY_OPEN_NO_DOTGIT;
        if (getboolfield(l, table_index, "from_env")) flags |= GIT_REPOSITORY_OPEN_FROM_ENV;
    }

    int code = git_repository_open_ext(&repo, path, flags, ceiling_dirs);
    if (code == GIT_ENOTFOUND) {
        lua_pushnil(l);
    } else if (code) {
        const git_error *err = git_error_last();
        luaL_error(l, err->message);
    } else {
        git_repository **udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_REPOSITORY);
        *udat = repo;
    }
    return 1;
}

/**
 * 获取仓库的真正路径，指定的路径可能是仓库的子目录，向上递归查找，找到第一个仓库位置
 *
 * @throw git_error
 * @return 仓库的路径
 */
static int lrepository_discover(lua_State *l) {
    git_buf out;

    const char *start_path = luaL_checkstring(l, 1);
    // 如果为 true，那么在探索父目录时检测到文件系统设备更改时，查找将不会停止。
    int across_fs = lua_toboolean(l, 2);
    // 向上递归到指定的路径时，停止查找。如果 cellng_dirs 为 NULL，等同于空字符串。
    const char *ceiling_dirs = lua_tostring(l, 3);
    LUA_GIT_ERROR(l, git_repository_discover(&out, start_path, across_fs, ceiling_dirs));
    lua_pushlstring(l, out.ptr, out.size);
    return 1;
}

/**
 * 获取仓库的 index
 *
 * @throw git_error
 * @return 仓库索引的句柄
 */
static int lrepository_index(lua_State *l) {
    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);

    git_index **index = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_INDEX);
    LUA_GIT_ERROR(l, git_repository_index(index, repo));
    return 1;
}

/**
 * 初始化 repository 模块
 */
void lrepository_minit(lua_State *l, int module_table) {
    struct luaL_Reg md_funcs[] = {
        {"init", lrepository_init},
        {"open", lrepository_open},
        {"open_ext", lrepository_open_ext},
        {"discover", lrepository_discover},
        {"index", lrepository_index},
        {NULL, NULL}
    };
    luaL_newlib(l, md_funcs);
    lua_setfield(l, module_table, "repository");
}
