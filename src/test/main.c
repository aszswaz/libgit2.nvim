#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libgen.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define LUA_ERROR(expr, state) \
    if (expr) { \
        exit_code = EXIT_FAILURE; \
        fputs(lua_tostring(state, -1), stderr); \
        fputs("\n", stderr); \
        goto finally; \
    }

extern int luaopen_libgit2(lua_State *l);

static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)ud;
    (void)osize;
    if (nsize == 0) {
        free(ptr);
        return NULL;
    } else {
        return realloc(ptr, nsize);
    }
}

int main(int argc, const char *argv[]) {
    int exit_code = EXIT_SUCCESS;

    lua_State *l = lua_newstate(l_alloc, NULL);
    if (!l) {
        fputs("The creation of Lua virtual machine failed.", stderr);
        goto finally;
    }
    // 加载标准库
    luaL_openlibs(l);
    // 加载 libgit2 模块
    luaopen_libgit2(l);
    lua_setglobal(l, "git");

    // 设置 arg
    int tbl_index = lua_gettop(l);
    lua_createtable(l, argc, 0);
    // 根据本文件的路径，得到 main.lua 文件的路径
    char script[PATH_MAX] = {};
    strcpy(script, __FILE__);
    dirname(script);
    strcat(script, "/main.lua");
    // 设置到 arg 中
    lua_pushstring(l, script);
    lua_rawseti(l, tbl_index, 0);
    // 将参数传递给脚本
    for (int i = 2; i < argc; i++) {
        lua_pushstring(l, argv[i]);
        lua_rawseti(l, tbl_index, i - 1);
    }
    lua_pushvalue(l, tbl_index);
    lua_setglobal(l, "arg");

    // 加载文件
    LUA_ERROR(luaL_loadfile(l, script), l);
    // 执行已加载的脚本文件
    LUA_ERROR(lua_pcall(l, 0, 0, 0), l);

finally:
    if (l) lua_close(l);
    return exit_code;
}
