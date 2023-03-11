# 宏管理

option(ALLOCATOR_LOGGER OFF)

if (${ENABLE_DEBUG})
    add_definitions(-DENABLE_LOGGER)
endif()
if (${ENABLE_ALLOCATOR_LOGGER})
    add_definitions(-DENABLE_ALLOCATOR_LOGGER)
endif()
