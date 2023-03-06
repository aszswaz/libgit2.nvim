#ifndef LIBGIT2_NVIM_LOGGER_H
#define LIBGIT2_NVIM_LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <lua.h>

#ifdef ENABLE_LOGGER
#define LUA_LOGGER(lua_state, ...) lua_logger(lua_state, __FILE_NAME__, __LINE__, __VA_ARGS__);
#else
#define LUA_LOGGER(lua_state, ...)
#endif

/**
 * 通过 lua 的 print 函数打印 log
 */
void lua_logger(lua_State *l, const char *file, int line, const char *format, ...);

#endif
