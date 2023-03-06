# 宏管理

option(ALLOCATOR_LOGGER OFF)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DENABLE_LOGGER)
    if (${ALLOCATOR_LOGGER} STREQUAL "ON")
        add_definitions(-DENABLE_ALLOCATOR_LOGGER)
    endif()
endif()
