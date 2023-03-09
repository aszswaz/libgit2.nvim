#ifndef LIBGIT2_NVIM_UTIL_H
#define LIBGIT2_NVIM_UTIL_H

#include <string.h>

#include <lua.h>
#include <git2.h>

void *new_userdata(lua_State *l, size_t sz, const char *metatable);

git_strarray *table2git_strarray(lua_State *l, int tbl_index);
git_strarray *str2git_strarray(lua_State *l, int index);

int getintfield(lua_State *l, int tbx, const char *field);
int getboolfield(lua_State *l, int tbx, const char *field);
void *getudatfield(lua_State *l, int tbx, const char *field, const char *tname);
const char *getstrfield(lua_State *l, int tbx, const char *field);

#endif
