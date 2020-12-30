cmake_minimum_required (VERSION 3.0)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/utils/cmake)

project(lgncapi C)

add_library(lgncopenapi SHARED IMPORTED GLOBAL)

set_target_properties(lgncopenapi PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES      ${CMAKE_CURRENT_SOURCE_DIR}/include
    IMPORTED_LOCATION                  ${CMAKE_CURRENT_SOURCE_DIR}/lib/liblgncopenapi.so
)

# Don't include samples if used as library
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(samples)
endif()