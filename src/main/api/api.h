#ifndef LIBGIT2_NVIM_API_H
#define LIBGIT2_NVIM_API_H

#include <git2.h>
#include <lua.h>
#include <lauxlib.h>

#include "../error.h"
#include "../metatables/metatables.h"
#include "../util/util.h"

/**
 * API 初始化
 */

void lrepository_minit(lua_State *l, int module);
void lindex_minit(lua_State *l, int module);
void lrevparse_minit(lua_State *l, int module);
void ltree_minit(lua_State *l, int module);
void lsignature_minit(lua_State *l, int module);
void lcommit_minit(lua_State *l, int module);
void lremote_minit(lua_State *l, int module);
void lpush_minit(lua_State *l, int module);
void lconst_minit(lua_State *l, int module);
void lstatus_minit(lua_State *l, int module);
#endif
