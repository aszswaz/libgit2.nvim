#ifndef LIBGIT2_NVIM_METATABLE_H
#define LIBGIT2_NVIM_METATABLE_H

#include <lua.h>

#define LUA_METATABLE_GIT_REPOSITORY   "git_repository"
#define LUA_METATABLE_GIT_INDEX        "git_index"
#define LUA_METATABLE_GIT_OBJECT       "git_object"
#define LUA_METATABLE_GIT_REFERENCE    "git_reference"
#define LUA_METATABLE_GIT_TREE         "git_tree"
#define LUA_METATABLE_GIT_OID          "git_oid"
#define LUA_METATABLE_GIT_SIGNATURE    "git_signature"
#define LUA_METATABLE_GIT_COMMIT       "git_commit"
#define LUA_METATABLE_GIT_REMOTE       "git_remote"
#define LUA_METATABLE_GIT_PUSH_OPTIONS "git_push_options"
#define LUA_METATABLE_GIT_STATUS_LIST  "git_status_list"
#define LUA_METATABLE_GIT_STATUS_ENTRY "git_status_entry"
#define LUA_METATABLE_GIT_DIFF_DELTA   "git_diff_delta"
#define LUA_METATABLE_GIT_DIFF_FILE    "git_diff_file"

#define LUA_METATABLE_GIT_TIME     "git_time"
#define LUA_METATABLE_GIT_STRARRAY "git_strarray"

/**
 * 给 libgit2 或 C 结构体创建对应的 metatable，
 * 这可以让结构体的内存被 lua 的 GC 管理，
 * 也可以从 lua 代码访问结构体的属性
 */
void lmetatables_init(lua_State *l);
#endif
