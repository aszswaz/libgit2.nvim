#ifndef LIBGIT2_NVIM_ALLOCATOR_H
#define LIBGIT2_NVIM_ALLOCATOR_H

#include <lua.h>

/**
 * 使用 lua 的内存分配器分配内存
 */

void set_allocator(lua_State *l);

/**
 * 分配内存
 */
void *lmalloc(size_t size, const char *file, int line);
void *lcalloc(size_t nmemb, size_t size, const char *file, int line);
void *lrealloc (void *ptr, size_t size, const char *file, int line);

/**
 * 复制字符串
 */
char *lstrdup(const char *str, const char *file, int line);
char *lstrndup(const char *str, size_t n, const char *file, int line);

/**
 * 截取字符串
 */
char *lsubstrdup(const char *str, size_t n, const char *file, int line);

/**
 * 分配数组的内存
 */
void *lreallocarray(void *ptr, size_t nelem, size_t elsize, const char *file, int line);
void *lmallocarray(size_t nelem, size_t elsize, const char *file, int line);

/**
 * 释放内存
 */
void lfree(void *ptr);

#endif
