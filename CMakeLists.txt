cmake_minimum_required(VERSION 3.22.2)
project("libgit2.nvim")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
# 生成 compile_commands.json 文件，一些用具会用它来进行语法辅助，比如 ccls
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
execute_process(COMMAND "ln" "-svf" "${CMAKE_BINARY_DIR}/compile_commands.json" "compile_commands.json")
# 设置 gcc 编译选项
set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g3 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")
set(CMAKE_C_STANDARD 17)
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")

# 扫描源文件
file(GLOB_RECURSE SOURCE_MAIN ./src/main/*.c)
file(GLOB_RECURSE SOURCE_TEST ./src/test/*.c)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
add_executable(${PROJECT_NAME} ${SOURCE_MAIN} ${SOURCE_TEST})
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCE_MAIN})
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_NAME git2)
    install(TARGETS ${PROJECT_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/lua")
endif()

include("cmake/depends.cmake")
include("cmake/macro.cmake")
