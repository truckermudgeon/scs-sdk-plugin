cmake_minimum_required(VERSION 3.14)

project(scs_telemetry_plugin
    VERSION 1.12.1
    DESCRIPTION "Cross-platform telemetry plugin for ATS/ETS2"
    LANGUAGES CXX
)

if (APPLE)
    # must target the same architecture that the macOS ATS/ETS2
    # executables target. may be able to change this if SCS
    # ships universal or arm64 binaries.
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
endif()

file(GLOB plugin_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/scs-telemetry/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/scs_sdk/*.cpp"
)

add_library(scs_telemetry_plugin SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/scs-telemetry/scs-telemetry.def"
    ${plugin_sources}
)

target_include_directories(scs_telemetry_plugin
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/scs-telemetry/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/scs_sdk/include
)

if (WIN32)
    target_compile_definitions(scs_telemetry_plugin PRIVATE _CRT_SECURE_NO_WARNINGS UNICODE)
    target_compile_options(scs_telemetry_plugin PRIVATE /W4 /wd4100 /permissive-)
elseif (UNIX AND NOT APPLE)
    # `shm_open` on linux requires `librt`
    target_link_libraries(scs_telemetry_plugin PRIVATE rt)
endif()

set_target_properties(scs_telemetry_plugin PROPERTIES
    OUTPUT_NAME "scs-telemetry"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin/"
)

target_compile_features(scs_telemetry_plugin PRIVATE cxx_std_14)
