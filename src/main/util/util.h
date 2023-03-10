#ifndef LIBGIT2_NVIM_UTIL_H
#define LIBGIT2_NVIM_UTIL_H

#include <string.h>

#include <lua.h>
#include <git2.h>

void *new_userdata(lua_State *l, size_t sz, const char *metatable);

git_strarray *table2git_strarray(lua_State *l, int tbl_index);
git_strarray *str2git_strarray(lua_State *l, int index);
void table2git_status_options(lua_State *l, int tbx, git_status_options *opt);

/**
 * 获取 table 的属性
 */
int getintfield(lua_State *l, int tbx, const char *field);
int getboolfield(lua_State *l, int tbx, const char *field);
void *getudatfield(lua_State *l, int tbx, const char *field, const char *tname);
const char *getstrfield(lua_State *l, int tbx, const char *field);
int get_int_field_or_default(lua_State *l, int tbx, const char *field, int default_value);
void *get_udatfield_or_default(lua_State *l, int tbx, const char *field, const char *metatable, void *default_value);

/**
 * 将 table 中 index 处的元素压入栈
 */
void getbyindex(lua_State *l, int tbx, int idx);
#endif
