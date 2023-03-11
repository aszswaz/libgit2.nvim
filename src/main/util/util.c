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
        getbyindex(l, tbl_index, i);
        array->strings[i] = LSTRDUP(luaL_checkstring(l, -1));
    }
    git_strarray **udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_STRARRAY);
    *udat = array;
    return array;
}

void table2git_status_options(lua_State *l, int tbx, git_status_options *opt) {
    luaL_checktype(l, tbx, LUA_TTABLE);

    opt->version = get_int_field_or_default(l, tbx, "version", GIT_STATUS_OPTIONS_VERSION);
    opt->show = get_int_field_or_default(l, tbx, "show", GIT_STATUS_SHOW_INDEX_AND_WORKDIR);
    opt->rename_threshold = get_int_field_or_default(l, tbx, "rename_threshold", 0);
    void *udat = get_udatfield_or_default(l, tbx, "baseline", LUA_METATABLE_GIT_TREE, NULL);
    if (udat) opt->baseline = *(git_tree **)udat;

    lua_getfield(l, tbx, "flags");
    if (lua_isnil(l, -1)) {
        opt->flags = GIT_STATUS_OPT_DEFAULTS;
    } else {
        int tbl_idx = lua_gettop(l);
        opt->flags = 0;
        if (getboolfield(l, tbl_idx, "include_untracked")) opt->flags |= GIT_STATUS_OPT_INCLUDE_UNTRACKED;
        if (getboolfield(l, tbl_idx, "include_ignored")) opt->flags |= GIT_STATUS_OPT_INCLUDE_IGNORED;
        if (getboolfield(l, tbl_idx, "include_unmodified")) opt->flags |= GIT_STATUS_OPT_INCLUDE_UNMODIFIED;
        if (getboolfield(l, tbl_idx, "exclude_submodules")) opt->flags |= GIT_STATUS_OPT_EXCLUDE_SUBMODULES;
        if (getboolfield(l, tbl_idx, "recurse_untracked_dirs")) opt->flags |= GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
        if (getboolfield(l, tbl_idx, "disable_pathspec_match")) opt->flags |= GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH;
        if (getboolfield(l, tbl_idx, "recurse_ignored_dirs")) opt->flags |= GIT_STATUS_OPT_RECURSE_IGNORED_DIRS;
        if (getboolfield(l, tbl_idx, "renames_head_to_index")) opt->flags |= GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX;
        if (getboolfield(l, tbl_idx, "renames_index_to_workdir")) opt->flags |= GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR;
        if (getboolfield(l, tbl_idx, "sort_case_sensitively")) opt->flags |= GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
        if (getboolfield(l, tbl_idx, "sort_case_insensitively")) opt->flags |= GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY;
        if (getboolfield(l, tbl_idx, "renames_from_rewrites")) opt->flags |= GIT_STATUS_OPT_RENAMES_FROM_REWRITES;
        if (getboolfield(l, tbl_idx, "no_refresh")) opt->flags |= GIT_STATUS_OPT_NO_REFRESH;
        if (getboolfield(l, tbl_idx, "update_index")) opt->flags |= GIT_STATUS_OPT_UPDATE_INDEX;
        if (getboolfield(l, tbl_idx, "include_unreadable")) opt->flags |= GIT_STATUS_OPT_INCLUDE_UNREADABLE;
        if (getboolfield(l, tbl_idx, "include_unreadable_as_untracked")) opt->flags |= GIT_STATUS_OPT_INCLUDE_UNREADABLE_AS_UNTRACKED;
    }

    lua_getfield(l, tbx, "pathspec");
    int tbl_idx = lua_gettop(l);
    if (!lua_isnil(l, tbl_idx)) {
        size_t tbl_size = lua_objlen(l, tbl_idx);
        opt->pathspec.count = tbl_size;
        size_t mem_size = tbl_size * sizeof(void *);
        opt->pathspec.strings = lua_newuserdata(l, mem_size);
        for (size_t i = 0; i < tbl_size; i++) {
            getbyindex(l, tbl_idx, i);
            opt->pathspec.strings[i] = (char *)luaL_checkstring(l, -1);
        }
    }
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
    return luaL_checkint(l, -1);
}
int getboolfield(lua_State *l, int tbx, const char *field) {
    lua_getfield(l, tbx, field);
    return lua_toboolean(l, -1);
}
void *getudatfield(lua_State *l, int tbx, const char *field, const char *tname) {
    lua_getfield(l, tbx, field);
    return luaL_checkudata(l, -1, tname);
}
const char *getstrfield(lua_State *l, int tbx, const char *field) {
    lua_getfield(l, tbx, field);
    return luaL_checkstring(l, -1);
}
int get_int_field_or_default(lua_State *l, int tbx, const char *field, int default_value) {
    lua_getfield(l, tbx, field);
    if (lua_isnil(l, -1)) {
        return default_value;
    }
    return luaL_checkint(l, -1);
}
void *get_udatfield_or_default(lua_State *l, int tbx, const char *field, const char *metatable, void *default_value) {
    lua_getfield(l, tbx, field);
    if (lua_isnil(l, -1)) {
        return default_value;
    }
    return luaL_checkudata(l, -1, metatable);
}

void getbyindex(lua_State *l, int tbx, int idx) {
    lua_pushinteger(l, idx + 1);
    lua_gettable(l, tbx);
}
