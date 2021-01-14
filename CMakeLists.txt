cmake_minimum_required(VERSION 3.1)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/utils/cmake)

project(lgncapi C)

set(PROJECT_NAME "lgncapi")
set(PROJECT_DESCRIPTION "NDK 2.0 Legacy APIs (LGNC Open API)")
set(PROJECT_AUTHOR "Mariotaku Lee")
set(PROJECT_URL "https://github.com/webosbrew/lgncapi")

set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 4)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(BIN_INSTALL_DIR bin CACHE STRING "Binary installation directory relative to the install prefix")
set(LIB_INSTALL_DIR lib CACHE STRING "Library installation directory relative to the install prefix")
set(INCLUDE_INSTALL_DIR include CACHE STRING "Header installation directory relative to the install prefix")

set(HEADERS 
    "include/lgnc_directaudio.h"
    "include/lgnc_directvideo.h"
    "include/lgnc_system.h"
)

set(SRC 
    "src/lgnc_directaudio.c"
    "src/lgnc_directvideo.c"
    "src/lgnc_system.c"
)

add_library(lgncopenapi ${HEADERS} ${SRC})

set_target_properties(lgncopenapi PROPERTIES
    PUBLIC_HEADER         ${HEADERS}
    VERSION               ${PROJECT_VERSION}
    SOVERSION             ${PROJECT_VERSION_MAJOR}
)

target_include_directories(lgncopenapi PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
)

target_link_libraries(lgncopenapi dl)

install(TARGETS lgncopenapi
    EXPORT        libndl-export
    RUNTIME       DESTINATION "${BIN_INSTALL_DIR}"
    LIBRARY       DESTINATION "${LIB_INSTALL_DIR}"
    ARCHIVE       DESTINATION "${LIB_INSTALL_DIR}"
    PUBLIC_HEADER DESTINATION "${INCLUDE_INSTALL_DIR}"
)

# Don't include samples if used as library
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(samples)
endif()