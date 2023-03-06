#ifndef LIBGIT2_NVIM_ERROR_H
#define LIBGIT2_NVIM_ERROR_H

#include <lauxlib.h>
#include <lua.h>

#define LUA_GIT_ERROR(lua_state, status) \
    if (status) { \
        const git_error *err = git_error_last(); \
        luaL_error(lua_state, err->message); \
    }

#endif
