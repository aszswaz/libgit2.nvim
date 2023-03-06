#include <string.h>

#include "api.h"

/**
 * 获取默认的用户签名
 */
static int lsignature_default(lua_State *l) {
    git_signature *sign;

    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);

    int code = git_signature_default(&sign, repo);
    if (code == GIT_ENOTFOUND) {
        lua_pushnil(l);
    } else if (code) {
        const git_error *err = git_error_last();
        luaL_error(l, err->message);
    } else {
        void **udat = new_userdata(l, sizeof(size_t), LUA_METATABLE_GIT_SIGNATURE);
        *udat = sign;
    }
    return 1;
}


void lsignature_minit(lua_State *l, int module_table) {
    luaL_Reg funcs01[] = {
        {"default", lsignature_default},
        {NULL, NULL}
    };
    luaL_newlib(l, funcs01);
    lua_setfield(l, module_table, "signature");
}
