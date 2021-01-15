cmake_minimum_required (VERSION 3.6)

include(WebOSPackage)

project(gst LANGUAGES C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLib REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GStreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 gstreamer-app-1.0)
pkg_check_modules(OpenGL REQUIRED IMPORTED_TARGET egl glesv2)
pkg_check_modules(json REQUIRED IMPORTED_TARGET json-c)

add_executable(sample-gst
    src/main.c
    src/callbacks.c
    src/gst_sample.c
    src/graphics.c
)
target_include_directories(sample-gst PRIVATE src)
target_link_libraries(sample-gst lgncopenapi PkgConfig::GLib PkgConfig::GStreamer PkgConfig::OpenGL PkgConfig::json)

target_compile_definitions(sample-gst PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_include_directories(sample-gst PRIVATE ${CMAKE_SOURCE_DIR}/utils/include)

set_target_properties(sample-gst PROPERTIES
    WEBOS_APPINFO_ID org.webosbrew.sample.lgncapi-gst
    WEBOS_APPINFO_VERSION 0.0.1
    WEBOS_APPINFO_VENDOR "Mariotaku Lee"
    WEBOS_APPINFO_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
    WEBOS_APPINFO_TITLE "LGNC GST"
    WEBOS_APPINFO_EXTRA "resolution=1280x720;uiRevision=2"
    WEBOS_PACKAGE_ASSETS "appconfig.json;lib"
)

target_webos_package(sample-gst)