include(WebOSPackage)

project(egl LANGUAGES C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(OpenGL REQUIRED IMPORTED_TARGET egl glesv2)

add_executable(sample-egl
    src/main.c
    src/callbacks.c
    src/graphics.c
)
target_include_directories(sample-egl PRIVATE src)
target_link_libraries(sample-egl lgncopenapi PkgConfig::OpenGL)

target_compile_definitions(sample-egl PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_include_directories(sample-egl PRIVATE ${CMAKE_SOURCE_DIR}/utils/include)

set_target_properties(sample-egl PROPERTIES
    WEBOS_APPINFO_ID org.webosbrew.sample.lgncapi-egl
    WEBOS_APPINFO_VERSION 0.0.1
    WEBOS_APPINFO_VENDOR "Mariotaku Lee"
    WEBOS_APPINFO_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
    WEBOS_APPINFO_TITLE "LGNC EGL"
    WEBOS_APPINFO_EXTRA "resolution=1280x720;uiRevision=2"
)

target_webos_package(sample-egl)