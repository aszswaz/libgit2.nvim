#include <stdio.h>
#include <stdlib.h>

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

    if (argc < 2) {
        fprintf(stderr, "Please specify the LUA script to be executed.\n");
        return EXIT_FAILURE;
    }

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
    char script[PATH_MAX] = {};
    realpath(argv[1], script);
    lua_createtable(l, argc, 0);
    int tbl_index = lua_gettop(l);
    lua_pushstring(l, script);
    lua_rawseti(l, tbl_index, 0);
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
