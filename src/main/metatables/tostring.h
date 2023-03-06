#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <git2.h>

#include "../metatables/metatables.h"
#include "../util/util.h"
#include "../error.h"

// 使用 sprintf 格式化字符串
#define add_string(fmt, value) {\
        size_t len = 0; \
        char str_buf[BUFSIZ] = {}; \
        len = sprintf(str_buf, fmt, value); \
        if (len < 0) luaL_error(l, strerror(len)); \
        luaL_addlstring(&buffer, str_buf, len); \
    }

// 调用指定的 metatable 中的 __tostring，将返回值添加到 luaL_Buffer
#define call_tostring(metatable, name, p) \
    if (p) { \
        luaL_getmetatable(l, metatable); \
        lua_getfield(l, -1, "__tostring"); \
        void **udat = new_userdata(l, sizeof(size_t), LUA_METATABLE_GIT_DIFF_FILE); \
        *udat = (void *)p; \
        lua_call(l, 1, 1); \
        luaL_addstring(&buffer, ", " name " = "); \
        luaL_addstring(&buffer, lua_tostring(l, -1)); \
    }

static int lm_tostring_signature(lua_State *l) {
    git_signature *sign = *(git_signature **)lua_touserdata(l, 1);
    const char *fmt = "{ name = '%s', email = '%s', when = { time = %d, offset = %d, sign = %c } }";
    lua_pushfstring(l, fmt, sign->name, sign->email, sign->when.time, sign->when.offset, sign->when.sign);
    return 1;
}

static int lm_tostring_time(lua_State *l) {
    git_time *time = lua_touserdata(l, 1);
    const char *fmt = "{ time = %d, offset = %d, sign = %c }";
    lua_pushfstring(l, fmt, time->time, time->offset, time->sign);
    return 1;
}

/**
 * git_status_entry 的属性信息
 */
static int lm_tostring_status_entry(lua_State *l) {
    git_status_entry *entry = NULL;

    entry = *(git_status_entry **)lua_touserdata(l, 1);

    luaL_Buffer buffer;
    luaL_buffinit(l, &buffer);
    luaL_addstring(&buffer, "{ ");
    luaL_addstring(&buffer, lua_pushfstring(l, "status = 0x%x", entry->status));

    if (entry->head_to_index) {
        call_tostring(
            LUA_METATABLE_GIT_DIFF_DELTA,
            "head_to_index",
            entry->head_to_index
        );
        luaL_addstring(&buffer, ", head_to_index = ");
        luaL_addstring(&buffer, lua_tostring(l, -1));
    } else {
        luaL_addstring(&buffer, ", head_to_index = nil");
    }
    if (entry->index_to_workdir) {
        call_tostring(
            LUA_METATABLE_GIT_DIFF_DELTA,
            "index_to_workdir",
            entry->index_to_workdir
        );
    } else {
        luaL_addstring(&buffer, ", index_to_workdir = nil");
    }

    luaL_addstring(&buffer, " }");
    luaL_pushresult(&buffer);
    return 1;
}

/**
 * git_diff_delta 的属性信息
 */
static int lm_tostring_diff_delta(lua_State *l) {
    luaL_Buffer buffer = {};
    git_diff_delta *delta = NULL;

    delta = *(git_diff_delta **)lua_touserdata(l, 1);

    luaL_buffinit(l, &buffer);
    luaL_addstring(&buffer, "{ ");

    add_string("status = %d", delta->status);
    add_string(", flags = 0x%x", delta->flags);
    add_string(", similarity = 0x%x", delta->similarity);
    add_string(", nfiles = 0x%x", delta->nfiles);

    call_tostring(
        LUA_METATABLE_GIT_DIFF_FILE,
        "old_file",
        &delta->old_file
    );
    call_tostring(
        LUA_METATABLE_GIT_DIFF_FILE,
        "new_file",
        &delta->new_file
    );

    luaL_addstring(&buffer, " }");
    luaL_pushresult(&buffer);
    return 1;
}

/**
 * 打印 git_diff_file
 */
static int lm_tostring_diff_file(lua_State *l) {
    luaL_Buffer buffer = {};
    char oidFmt[GIT_OID_HEXSZ + 1] = {};

    git_diff_file *file = *(git_diff_file **)lua_touserdata(l, 1);

    luaL_buffinit(l, &buffer);

    luaL_addstring(&buffer, "{ ");
    LUA_GIT_ERROR(l, git_oid_fmt(oidFmt, &file->id));
    add_string("id = 0x%s", oidFmt);
    add_string(", path = \"%s\"", file->path);
    add_string(", size = %lu", file->size);
    add_string(", flags = 0x%x", file->flags);
    add_string(", mode = %d", file->mode);
    add_string(", id_abbrev = %d", file->id_abbrev);
    luaL_addstring(&buffer, " }");

    luaL_pushresult(&buffer);
    return 1;
}

/**
 * 打印 git_strarray
 */
static int lm_tostring_strarray(lua_State *l) {
    luaL_Buffer buffer = {};

    git_strarray *array = *(git_strarray **)lua_touserdata(l, 1);

    luaL_buffinit(l, &buffer);
    luaL_addstring(&buffer, "{ ");
    for (int i = 0; i < array->count; i++) {
        luaL_addstring(&buffer, "\"");
        luaL_addstring(&buffer, array->strings[i]);
        luaL_addstring(&buffer, "\"");
        if (i < array->count - 1) {
            luaL_addstring(&buffer, ", ");
        }
    }
    luaL_addstring(&buffer, " }");
    luaL_pushresult(&buffer);
    return 1;
}
