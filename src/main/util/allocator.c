#include <string.h>

#include <git2.h>
#include <git2/sys/alloc.h>

#include "allocator.h"
#include "../error.h"

#ifdef ENABLE_ALLOCATOR_LOGGER
#include <stdio.h>
#define LOGGER(pointer) printf("%s at %s:%d, pointer: %p\n", __func__, file, line, pointer)
#else
#define LOGGER(pointer)
#endif

lua_Alloc lalloc = NULL;
void *lalloc_data = NULL;

void set_allocator(lua_State *l) {
    lalloc = lua_getallocf(l, &lalloc_data);

    git_allocator git_alloc = {
        .gmalloc = lmalloc,
        .gcalloc = lcalloc,
        .gstrdup = lstrdup,
        .gstrndup = lstrndup,
        .gsubstrdup = lsubstrdup,
        .grealloc = lrealloc,
        .greallocarray = lreallocarray,
        .gmallocarray = lmallocarray,
        .gfree = lfree
    };
    LUA_GIT_ERROR(l, git_libgit2_opts(GIT_OPT_SET_ALLOCATOR, &git_alloc));
}

void *lmalloc(size_t size, const char *file, int line) {
    if (!size) return NULL;
    void *pointer = lalloc(lalloc_data, NULL, 0, size);
    LOGGER(pointer);
    if (!pointer) git_error_set_oom();
    return pointer;
}

void *lcalloc(size_t nmemb, size_t size, const char *file, int line) {
    if (!nmemb || !size) return 0;
    size_t mem_size = nmemb * size;
    void *pointer = lalloc(lalloc_data, NULL, 0, mem_size);
    LOGGER(pointer);
    if (!pointer) git_error_set_oom();
    memset(pointer, 0, mem_size);
    return pointer;
}

void *lrealloc(void *ptr, size_t size, const char *file, int line) {
    if (!size) return NULL;
    void *pointer = lalloc(lalloc_data, ptr, 0, size);
    LOGGER(pointer);
    if (!pointer) git_error_set_oom();
    return pointer;
}

char *lstrdup(const char *str, const char *file, int line) {
    if (!str) return NULL;
    char *pointer = lalloc(lalloc_data, NULL, 0, strlen(str) + 1);
    LOGGER(pointer);
    if (!pointer)
        git_error_set_oom();
    else
        strcpy(pointer, str);
    return pointer;
}

char *lstrndup(const char *str, size_t n, const char *file, int line) {
    if (!str || !n) return NULL;
    char *pointer = lalloc(lalloc_data, NULL, 0, n + 1);
    LOGGER(pointer);
    if (!pointer)
        git_error_set_oom();
    else
        strncpy(pointer, str, n);
    return pointer;
}

char *lsubstrdup(const char *str, size_t n, const char *file, int line) {
    if (!str || !n) return NULL;
    char *pointer = lalloc(lalloc_data, NULL, 0, n + 1);
    LOGGER(pointer);
    if (!pointer) {
        git_error_set_oom();
    } else {
        memcpy(pointer, str, n);
        pointer[n] = 0;
    }
    return pointer;
}

void *lreallocarray(void *ptr, size_t nelem, size_t elsize, const char *file, int line) {
    if (!nelem || !elsize) return NULL;
    void *pointer = lalloc(lalloc_data, ptr, 0, nelem * elsize);
    LOGGER(pointer);
    if (!pointer) git_error_set_oom();
    return pointer;
}

void *lmallocarray(size_t nelem, size_t elsize, const char *file, int line) {
    if (!nelem || !elsize) return NULL;
    void *pointer = lalloc(lalloc_data, NULL, 0, nelem * elsize);
    LOGGER(pointer);
    if (!pointer) git_error_set_oom();
    return pointer;
}

void lfree(void *ptr) {
    #ifdef ENABLE_ALLOCATOR_LOGGER
    printf("%s, pointer: %p\n", __func__, ptr);
    #endif
    if (ptr) {
        lalloc(lalloc_data, ptr, 0, 0);
    }
}
