# 添加共享库，除了 luajit，其他库都是 libgit2 所必需的
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED luajit)
pkg_check_modules(LIBGIT2 REQUIRED libgit2)

include_directories(
    ${LUAJIT_INCLUDE_DIRS}
)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# 设置依赖项
target_link_libraries(
    ${PROJECT_NAME}
    ${LUAJIT_LIBRARIES}
    ${LIBGIT2_LIBRARIES}
)
