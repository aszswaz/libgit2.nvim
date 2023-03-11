#include <stdint.h>
#include <string.h>

#include "api.h"
#include "../util/allocator.h"

/**
 * 获取文件状态信息
 */
static int lstatus_list_new(lua_State *l) {
#define opt_idx 2

    git_status_options opt = {};

    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);

    if (!lua_isnil(l, opt_idx)) {
        opt.version = getintfield(l, opt_idx, "version");
        opt.show = getintfield(l, opt_idx, "show");
        opt.rename_threshold = getintfield(l, opt_idx, "rename_threshold");
        void *udat = getudatfield(l, opt_idx, "baseline", LUA_METATABLE_GIT_TREE);
        if (udat) opt.baseline = *(git_tree **)udat;

        if (getboolfield(l, opt_idx, "include_untracked")) opt.flags |= GIT_STATUS_OPT_INCLUDE_UNTRACKED;
        if (getboolfield(l, opt_idx, "include_ignored")) opt.flags |= GIT_STATUS_OPT_INCLUDE_IGNORED;
        if (getboolfield(l, opt_idx, "include_unmodified")) opt.flags |= GIT_STATUS_OPT_INCLUDE_UNMODIFIED;
        if (getboolfield(l, opt_idx, "exclude_submodules")) opt.flags |= GIT_STATUS_OPT_EXCLUDE_SUBMODULES;
        if (getboolfield(l, opt_idx, "recurse_untracked_dirs")) opt.flags |= GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
        if (getboolfield(l, opt_idx, "disable_pathspec_match")) opt.flags |= GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH;
        if (getboolfield(l, opt_idx, "recurse_ignored_dirs")) opt.flags |= GIT_STATUS_OPT_RECURSE_IGNORED_DIRS;
        if (getboolfield(l, opt_idx, "renames_head_to_index")) opt.flags |= GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX;
        if (getboolfield(l, opt_idx, "renames_index_to_workdir")) opt.flags |= GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR;
        if (getboolfield(l, opt_idx, "sort_case_sensitively")) opt.flags |= GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
        if (getboolfield(l, opt_idx, "sort_case_insensitively")) opt.flags |= GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY;
        if (getboolfield(l, opt_idx, "renames_from_rewrites")) opt.flags |= GIT_STATUS_OPT_RENAMES_FROM_REWRITES;
        if (getboolfield(l, opt_idx, "no_refresh")) opt.flags |= GIT_STATUS_OPT_NO_REFRESH;
        if (getboolfield(l, opt_idx, "update_index")) opt.flags |= GIT_STATUS_OPT_UPDATE_INDEX;
        if (getboolfield(l, opt_idx, "include_unreadable")) opt.flags |= GIT_STATUS_OPT_INCLUDE_UNREADABLE;
        if (getboolfield(l, opt_idx, "include_unreadable_as_untracked")) opt.flags |= GIT_STATUS_OPT_INCLUDE_UNREADABLE_AS_UNTRACKED;

        lua_getfield(l, opt_idx, "pathspec");
        int tbl_idx = lua_gettop(l);
        if (!lua_isnil(l, tbl_idx)) {
            size_t tbl_size = lua_objlen(l, tbl_idx);
            opt.pathspec.count = tbl_size;
            size_t mem_size = tbl_size * sizeof(void *);
            opt.pathspec.strings = lua_newuserdata(l, mem_size);
            for (size_t i = 0; i < tbl_size; i++) {
                getbyindex(l, tbl_idx, i);
                opt.pathspec.strings[i] = (char *)luaL_checkstring(l, -1);
            }
        }
    }

    git_status_list **list = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_STATUS_LIST);
    LUA_GIT_ERROR(l, git_status_list_new(list, repo, &opt));
    return 1;
}

/**
 * 初始化 status 模块
 */
void lstatus_minit(lua_State *l, int module) {
    lua_createtable(l, 1, 1);

    luaL_setfuncs(l, ((luaL_Reg []) {
        {"list_new", lstatus_list_new},
        {NULL, NULL}
    }), 0);

    lua_setfield(l, module, "status");
}
