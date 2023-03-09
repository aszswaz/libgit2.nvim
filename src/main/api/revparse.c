#include "api.h"

static int lrevparse_ext(lua_State *l) {
    git_object *obj;
    git_reference *ref;

    git_repository *repo = *(git_repository **)luaL_checkudata(l, 1, LUA_METATABLE_GIT_REPOSITORY);
    const char *spec = luaL_checkstring(l, 2);

    int code = git_revparse_ext(&obj, &ref, repo, spec);
    if (code == GIT_ENOTFOUND) {
        // 这是一个空仓库，没有 commit
        lua_pushnil(l);
        lua_pushnil(l);
    } else if (code) {
        // 其他 git 异常
        const git_error *err = git_error_last();
        luaL_error(l, err->message);
    } else {
        void **udat;
        udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_OBJECT);
        *udat = (void *)obj;
        udat = new_userdata(l, sizeof(void *), LUA_METATABLE_GIT_REFERENCE);
        *udat = (void *)ref;
    }
    return 2;
}

void lrevparse_minit(lua_State *l, int module_table) {
    luaL_Reg funcs[] = {
        {"ext", lrevparse_ext},
        {NULL, NULL}
    };
    luaL_newlib(l, funcs);
    lua_setfield(l, module_table, "revparse");
}
