#include "logger.h"

void lua_logger(lua_State *l, const char *file, int line, const char *format, ...) {
    #ifdef ENABLE_LOGGER
    va_list vargs;
    char buffer[BUFSIZ] = {};

    snprintf(buffer, sizeof(buffer) - 1, "%s: %-5d %s", file, line, format);
    lua_getglobal(l, "print");
    va_start(vargs, format);
    lua_pushvfstring(l, buffer, vargs);
    va_end(vargs);
    lua_call(l, 1, 0);
    #endif
}
