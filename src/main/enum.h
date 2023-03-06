#ifndef LIBGIT2_NVIM_ENUM_H
#define LIBGIT2_NVIM_ENUM_H

#include <lua.h>
#include <git2.h>

#define SET_ENUM(macro) \
    lua_pushinteger(l, GIT_##macro); \
    lua_setfield(l, tbl_index, #macro)

/**
 * 将 libgit2 通过宏或 enum 定义的常量传递给 lua
 */
static void lenum_minit(lua_State *l, int module) {
    lua_createtable(l, 20, 20);
    int tbl_index = lua_gettop(l);

    // git_object_t
    SET_ENUM(OBJECT_ANY);
    SET_ENUM(OBJECT_INVALID);
    SET_ENUM(OBJECT_COMMIT);
    SET_ENUM(OBJECT_TREE);
    SET_ENUM(OBJECT_BLOB);
    SET_ENUM(OBJECT_TAG);
    SET_ENUM(OBJECT_OFS_DELTA);
    SET_ENUM(OBJECT_REF_DELTA);

    // git_config_level_t
    SET_ENUM(CONFIG_LEVEL_PROGRAMDATA);
    SET_ENUM(CONFIG_LEVEL_SYSTEM);
    SET_ENUM(CONFIG_LEVEL_XDG);
    SET_ENUM(CONFIG_LEVEL_GLOBAL);
    SET_ENUM(CONFIG_LEVEL_LOCAL);
    SET_ENUM(CONFIG_LEVEL_APP);
    SET_ENUM(CONFIG_HIGHEST_LEVEL);

    // git_status_show_t
    SET_ENUM(STATUS_SHOW_INDEX_AND_WORKDIR);
    SET_ENUM(STATUS_SHOW_INDEX_ONLY);
    SET_ENUM(STATUS_SHOW_WORKDIR_ONLY);
    // git_status_t
    SET_ENUM(STATUS_CURRENT);
    SET_ENUM(STATUS_INDEX_NEW);
    SET_ENUM(STATUS_INDEX_MODIFIED);
    SET_ENUM(STATUS_INDEX_DELETED);
    SET_ENUM(STATUS_INDEX_RENAMED);
    SET_ENUM(STATUS_INDEX_TYPECHANGE);
    SET_ENUM(STATUS_WT_NEW);
    SET_ENUM(STATUS_WT_MODIFIED);
    SET_ENUM(STATUS_WT_DELETED);
    SET_ENUM(STATUS_WT_TYPECHANGE);
    SET_ENUM(STATUS_WT_RENAMED);
    SET_ENUM(STATUS_WT_UNREADABLE);
    SET_ENUM(STATUS_IGNORED);
    SET_ENUM(STATUS_CONFLICTED);

    // other
    SET_ENUM(PUSH_OPTIONS_VERSION);
    SET_ENUM(STATUS_OPTIONS_VERSION);

    lua_pushvalue(l, tbl_index);
    lua_setfield(l, module, "enum");
}
#endif
