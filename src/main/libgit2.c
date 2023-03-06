#include "string.h"

#include "api/api.h"
#include "error.h"
#include "metatables/metatables.h"
#include "enum.h"
#include "logger.h"
#include "util/allocator.h"

#define set_boolean_filed(state, tbl_index, field, value) \
    lua_pushboolean(state, value); \
    lua_setfield(state, tbl_index, field);

#define set_int_field(state, tbl_index, field, value) \
    lua_pushinteger(state, value); \
    lua_setfield(state, tbl_index, field);

#define opt_equal(doption) !strcmp(option, doption)

#define get_int_opt(doption) \
    if (git_libgit2_opts(GIT_OPT_GET_##doption, &number)) { \
        lua_pushnil(l); \
    } else { \
        lua_pushinteger(l, number); \
    }
#define get_str_opt(doption) \
    if (git_libgit2_opts(GIT_OPT_GET_##doption, &buffer)) { \
        lua_pushnil(l); \
    } else { \
        lua_pushlstring(l, buffer.ptr, buffer.size); \
    }

/**
 * 初始化 libgit2
 *
 * @return 初始化次数
 */
int llibgit2_init(lua_State *l) {
    int times = git_libgit2_init();
    lua_pushnumber(l, times);
    return 1;
}

/**
 * 销毁 libgit2，libgit2 被初始化多少次，就需要销毁多少次
 *
 * @return 还需要被销毁 N 次
 */
int llibgit2_shutdown(lua_State *l) {
    int times = git_libgit2_shutdown();
    lua_pushnumber(l, times);
    return 1;
}

/**
 * 查询 libgit2 支持的功能
 *
 * @return 返回一个有如下属性的对象：
 *              threads：是否支持多线程
 *              https：  是否支持 https 协议
 *              ssh：    是否支持 ssh 协议
 *              nsec：   是否支持纳秒
 */
int llibgit2_features(lua_State *l) {
    int result = git_libgit2_features();

    lua_createtable(l, 4, 4);
    int tbl_index = lua_gettop(l);
    set_boolean_filed(l, tbl_index, "threads", result & GIT_FEATURE_THREADS);
    set_boolean_filed(l, tbl_index, "https", result & GIT_FEATURE_HTTPS);
    set_boolean_filed(l, tbl_index, "ssh", result & GIT_FEATURE_SSH);
    set_boolean_filed(l, tbl_index, "nsec", result & GIT_FEATURE_NSEC);
    lua_pushvalue(l, tbl_index);
    return 1;
}

/**
 * 获取 libgit2 的版本信息
 *
 * @return 返回一个对象，属性如下：
 *          major：   主版本号
 *          minor：   次要版本号
 *          revision：修订号
 */
int llibgit2_version(lua_State *l) {
    int major = 0, minor = 0, rev = 0;

    LUA_GIT_ERROR(l, git_libgit2_version(&major, &minor, &rev));
    lua_createtable(l, 3, 3);
    int tbl_index = lua_gettop(l);
    set_int_field(l, tbl_index, "major", major);
    set_int_field(l, tbl_index, "minor", minor);
    set_int_field(l, tbl_index, "revision", rev);
    lua_pushvalue(l, tbl_index);
    return 1;
}

/**
 * 返回当前正在使用的 libgit2 库的预发布状态。
 * 对于活跃开发期间的夜间构建，这将是“alpha”。
 * 发布可能有“测试版”或发布候选（“rc1”、“rc2”等）预发布。
 * 对于最终版本，此函数返回 nil。
 */
int llibgit2_prerelease(lua_State *l) {
    const char *release = git_libgit2_prerelease();
    if (release) {
        lua_pushstring(l, release);
    } else {
        lua_pushnil(l);
    }
    return 1;
}

/**
 * 获取选项
 */
int llibgit2_get_option(lua_State *l) {
    size_t number = 0;
    git_buf buffer = {NULL, 0, 0};

    const char *option = luaL_checkstring(l, 1);

    if (opt_equal("mwindow_size")) {
        get_int_opt(MWINDOW_SIZE);
    } else if (opt_equal("mwindow_mapped_limit")) {
        get_int_opt(MWINDOW_MAPPED_LIMIT);
    } else if (opt_equal("search_path")) {
        int level = luaL_checkint(l, 2);
        if (git_libgit2_opts(GIT_OPT_GET_SEARCH_PATH, level, &buffer)) {
            lua_pushnil(l);
        } else {
            lua_pushlstring(l, buffer.ptr, buffer.size);
        }
    } else if (opt_equal("cached_memory")) {
        ssize_t current, allowed;
        if (git_libgit2_opts(GIT_OPT_GET_CACHED_MEMORY, &current, &allowed)) {
            lua_pushnil(l);
            lua_pushnil(l);
        } else {
            lua_pushinteger(l, current);
            lua_pushinteger(l, allowed);
        }
        return 2;
    } else if (opt_equal("template_path")) {
        get_str_opt(TEMPLATE_PATH);
    } else if (opt_equal("user_agent")) {
        get_str_opt(USER_AGENT);
    } else if (opt_equal("windows_sharemode")) {
        get_int_opt(WINDOWS_SHAREMODE);
    } else if (opt_equal("pack_max_objects")) {
        get_int_opt(PACK_MAX_OBJECTS);
    } else if (opt_equal("mwindow_file_limit")) {
        get_int_opt(MWINDOW_FILE_LIMIT);
    } else if (opt_equal("extensions")) {
        git_strarray strs = {NULL, 0};
        if (git_libgit2_opts(GIT_OPT_GET_EXTENSIONS, &strs)) {
            lua_pushnil(l);
        } else {
            git_strarray_dispose(&strs);
        }
    } else if (opt_equal("owner_validation")) {
        get_int_opt(OWNER_VALIDATION);
    } else {
        luaL_error(l, "unknown option: %s", option);
    }
    return 1;
}

/**
 * 设置选项
 */
int llibgit2_set_option(lua_State *l) {
    const char *option = luaL_checkstring(l, 1);

#define set_int_opt(macro) \
    size_t number = luaL_checkint(l, 2); \
    LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_SET_##macro, number))
#define set_str_opt(macro) \
    const char *str = luaL_checkstring(l, 2); \
    LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_SET_##macro, str))
#define enable_opt(macro) \
    int enable = lua_toboolean(l, 2); \
    LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_ENABLE_##macro, enable))

    if (opt_equal("mwindow_size")) {
        set_int_opt(MWINDOW_SIZE);
    } else if (opt_equal("mwindow_mapped_limit")) {
        set_int_opt(MWINDOW_MAPPED_LIMIT);
    } else if (opt_equal("search_path")) {
        int level = luaL_checkint(l, 2);
        const char *search_path = luaL_checkstring(l, 3);
        LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_SET_SEARCH_PATH, level, search_path));
    } else if (opt_equal("cache_object_limit")) {
        git_object_t type = luaL_checkint(l, 2);
        size_t size = luaL_checkint(l, 3);
        LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_SET_CACHE_OBJECT_LIMIT, type, size));
    } else if (opt_equal("cache_max_size")) {
        set_int_opt(CACHE_MAX_SIZE);
    } else if (opt_equal("template_path")) {
        set_str_opt(TEMPLATE_PATH);
    } else if (opt_equal("ssl_cert_locations")) {
        const char *file = lua_tostring(l, 2), *path = lua_tostring(l, 3);
        if (!(file || path)) {
            luaL_error(l, "file and path cannot both be nil");
        }
        LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_SET_SSL_CERT_LOCATIONS, file, path));
    } else if (opt_equal("user_agent")) {
        set_str_opt(USER_AGENT);
    } else if (opt_equal("ssl_ciphers")) {
        set_str_opt(SSL_CIPHERS);
    } else if (opt_equal("windows_sharemode")) {
        set_int_opt(WINDOWS_SHAREMODE);
    } else if (opt_equal("pack_max_objects")) {
        set_int_opt(PACK_MAX_OBJECTS);
    } else if (opt_equal("mwindow_file_limit")) {
        set_int_opt(MWINDOW_FILE_LIMIT);
    } else if (opt_equal("odb_packed_priority")) {
        set_int_opt(ODB_PACKED_PRIORITY);
    } else if (opt_equal("odb_loose_priority")) {
        set_int_opt(ODB_LOOSE_PRIORITY);
    } else if (opt_equal("extensions")) {
        /*
         * 设置支持的 git 拓展
         * git_libgit2_opts 内部会复制一份字符串副本，所以不需要担心 LUA GC 会造成影响
         */
        git_strarray *strs = table2git_strarray(l, 2);
        LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_SET_EXTENSIONS, strs->strings, strs->count));
    } else if (opt_equal("owner_validation")) {
        int enable = lua_toboolean(l, 2);
        LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_SET_OWNER_VALIDATION, enable));
    } else if (opt_equal("caching")) {
        enable_opt(CACHING);
    } else if (opt_equal("strict_object_creation")) {
        enable_opt(STRICT_OBJECT_CREATION);
    } else if (opt_equal("strict_symbolic_ref_creation")) {
        enable_opt(STRICT_SYMBOLIC_REF_CREATION);
    } else if (opt_equal("ofs_delta")) {
        enable_opt(OFS_DELTA);
    } else if (opt_equal("fsync_gitdir")) {
        enable_opt(FSYNC_GITDIR);
    } else if (opt_equal("strict_hash_verification")) {
        enable_opt(STRICT_HASH_VERIFICATION);
    } else if (opt_equal("unsaved_index_safety")) {
        enable_opt(UNSAVED_INDEX_SAFETY);
    } else if (opt_equal("http_expect_continue")) {
        enable_opt(HTTP_EXPECT_CONTINUE);
    } else if (opt_equal("disable_pack_keep_file_checks")) {
        int enable = lua_toboolean(l, 2);
        LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_DISABLE_PACK_KEEP_FILE_CHECKS, enable));
    } else {
        luaL_error(l, "unknown option: %s", option);
    }
    return 0;
}

/**
  初始化模块
  */
int luaopen_libgit2(lua_State *l) {
    // 拓展 lua 的站大小到 top + N 个元素
    luaL_checkstack(l, 100, "lua stack expansion failed");
    // 创建一个新的表，该表作为模块使用
    lua_createtable(l, 50, 50);
    // 获取表在栈中的位置
    const int module = lua_gettop(l);

    // 设置模块函数
    luaL_setfuncs(l, ((luaL_Reg []) {
        {"init", llibgit2_init},
        {"shutdown", llibgit2_shutdown},
        {"features", llibgit2_features},
        {"version", llibgit2_version},
        {"prerelease", llibgit2_prerelease},
        {"get_option", llibgit2_get_option},
        {"set_option", llibgit2_set_option},
        {NULL, NULL}
    }), 0);

    // 设置 libgit2 的内存分配器
    set_allocator(l);

    // 初始化模块的其他部分
    lenum_minit(l, module);
    lmetatables_init(l);
    lrepository_minit(l, module);
    lindex_minit(l, module);
    lrevparse_minit(l, module);
    ltree_minit(l, module);
    lsignature_minit(l, module);
    lcommit_minit(l, module);
    lremote_minit(l, module);
    lpush_minit(l, module);
    lstatus_minit(l, module);

    // 将模块的表放到栈顶，也就是设置返回值
    lua_pushvalue(l, module);
    return 1;
}
