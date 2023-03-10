#include <string.h>

#include <git2.h>
#include <lua.h>
#include <lauxlib.h>

#include "../util/util.h"
#include "../metatables/metatables.h"

#define field_equal(dfield) !strcmp(field, dfield)

/**
 * 获取 git_signature 的属性
 */
static int lm_index_signature(lua_State *l) {
    git_signature *sign = *(git_signature **)lua_touserdata(l, 1);
    const char *field = luaL_checkstring(l, 2);

    if (field_equal("name")) {
        lua_pushstring(l, sign->name);
    } else if (field_equal("email")) {
        lua_pushstring(l, sign->email);
    } else if (field_equal("when")) {
        git_time *udat = new_userdata(l, sizeof(git_time), LUA_METATABLE_GIT_TIME);
        *udat = sign->when;
    } else {
        lua_pushnil(l);
    }
    return 1;
}

/**
 * 获取 git_time 的属性
 */
static int lm_index_time(lua_State *l) {
    git_time *time = lua_touserdata(l, 1);
    const char *field = luaL_checkstring(l, 2);

    if (field_equal("time")) {
        lua_pushinteger(l, time->time);
    } else if (field_equal("offset")) {
        lua_pushinteger(l, time->offset);
    } else if (field_equal("sign")) {
        lua_pushlstring(l, &time->sign, 1);
    } else {
        lua_pushnil(l);
    }
    return 1;
}

/**
 * 获取 git_status_list 中元素
 */
static int lm_index_status_list(lua_State *l) {
    git_status_list *list = *(git_status_list **)lua_touserdata(l, 1);
    int idx = luaL_checkint(l, 2);

    if (idx < 1) {
        lua_pushnil(l);
        return 1;
    }

    const git_status_entry *entry = git_status_byindex(list, idx - 1);
    if (entry) {
        const git_status_entry **udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_STATUS_ENTRY);
        *udat = entry;
    } else {
        lua_pushnil(l);
    }
    return 1;
}

/**
 * 获取 git_status_entry 的属性值
 */
static int lm_index_status_entry(lua_State *l) {
    const git_status_entry *entry = *(git_status_entry **)lua_touserdata(l, 1);
    const char *field = luaL_checkstring(l, 2);

    if (field_equal("status")) {
        lua_pushinteger(l, entry->status);
    } else if (field_equal("head_to_index") && entry->head_to_index) {
        git_diff_delta **udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_DIFF_DELTA);
        *udat = entry->head_to_index;
    } else if (field_equal("index_to_workdir") && entry->index_to_workdir) {
        git_diff_delta **udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_DIFF_DELTA);
        *udat = entry->index_to_workdir;
    } else {
        lua_pushnil(l);
    }
    return 1;
}

/**
 * 访问 git_diff_delta
 */
static int lm_index_diff_delta(lua_State *l) {
    git_diff_delta *delta = *(git_diff_delta **)lua_touserdata(l, 1);
    const char *field = luaL_checkstring(l, 2);

    if (field_equal("status")) {
        lua_pushinteger(l, delta->status);
    } else if (field_equal("flags")) {
        lua_pushinteger(l, delta->flags);
    } else if (field_equal("similarity")) {
        lua_pushinteger(l, delta->similarity);
    } else if (field_equal("nfiles")) {
        lua_pushinteger(l, delta->nfiles);
    } else if (field_equal("new_file")) {
        git_diff_file **file = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_DIFF_FILE);
        *file = &delta->new_file;
    } else if (field_equal("old_file")) {
        git_diff_file **file = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_DIFF_FILE);
        *file = &delta->old_file;
    }
    return 1;
}

/**
 * 访问 git_diff_file
 */
static int lm_index_diff_file(lua_State *l) {
    git_diff_file *file = *(git_diff_file **)lua_touserdata(l, 1);
    const char *field = luaL_checkstring(l, 2);

    if (field_equal("id")) {
        char oid[GIT_OID_HEXSZ] = {};
        git_oid_fmt(oid, &file->id);
        lua_pushstring(l, oid);
    } else if (field_equal("path")) {
        lua_pushstring(l, file->path);
    } else if (field_equal("size")) {
        lua_pushinteger(l, file->size);
    } else if (field_equal("flags")) {
        lua_pushinteger(l, file->flags);
    } else if (field_equal("mode")) {
        lua_pushinteger(l, file->mode);
    } else if (field_equal("id_abbrev")) {
        lua_pushinteger(l, file->id_abbrev);
    }
    return 1;
}

/**
 * 访问 git_strarray
 */
static int lm_index_strarray(lua_State *l) {
    git_strarray *array = *(git_strarray **)lua_touserdata(l, 1);
    size_t index = luaL_checkint(l, 2);
    if (index >= 1 && index <= array->count) {
        lua_pushstring(l, array->strings[index - 1]);
    } else {
        lua_pushnil(l);
    }
    return 1;
}
