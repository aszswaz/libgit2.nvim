#include <lua.h>
#include "git2.h"

#include "../logger.h"

static int lm_gc_repository(lua_State *l) {
    git_repository *rep = *(git_repository **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, rep);
    if (!rep) return 0;
    git_repository_free(rep);
    return 0;
}

static int lm_gc_index(lua_State *l) {
    git_index *index = *(git_index **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, index);
    if (!index) return 0;
    git_index_free(index);
    return 0;
}

static int lm_gc_tree(lua_State *l) {
    git_tree *tree = *(git_tree **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, tree);
    git_tree_free(tree);
    return 0;
}

static int lm_gc_object(lua_State *l) {
    git_object *obj = *(git_object **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, obj);
    git_object_free(obj);
    return 0;
}

static int lm_gc_reference(lua_State *l) {
    git_reference *ref = *(git_reference **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, ref);
    git_reference_free(ref);
    return 0;
}

static int lm_gc_remote(lua_State *l) {
    git_remote *remote = *(git_remote **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, remote);
    git_remote_free(remote);
    return 0;
}

static int lm_gc_commit(lua_State *l) {
    git_commit *c = *(git_commit **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, c);
    git_commit_free(c);
    return 0;
}

static int lm_gc_signature(lua_State *l) {
    git_signature *sign = *(git_signature **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, sign);
    git_signature_free(sign);
    return 0;
}

static int lm_gc_status_list(lua_State *l) {
    git_status_list *list = *(git_status_list **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, list);
    git_status_list_free(list);
    return 0;
}

static int lm_gc_strarray(lua_State *l) {
    git_strarray *array = *(git_strarray **)lua_touserdata(l, 1);
    LUA_LOGGER(l, "%s: %p", __func__, array);
    git_strarray_dispose(array);
    free(array);
    return 0;
}
