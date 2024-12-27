cmake_minimum_required(VERSION 3.10)

project(WebGPU-Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OUTPUT_DIR_PLATFORM "unknown")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OUTPUT_DIR_PLATFORM "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(OUTPUT_DIR_PLATFORM "linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OUTPUT_DIR_PLATFORM "mac")
endif()

set(OUTPUT_DIR_CONFIG "unknown")
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(OUTPUT_DIR_CONFIG "debug")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set(OUTPUT_DIR_CONFIG "release")
endif()

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/export/${OUTPUT_DIR_PLATFORM}/${OUTPUT_DIR_CONFIG}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}")

set(SOURCES
    src/main.cpp
    src/engine/core/glfw_context.cpp
    src/engine/core/window.cpp
    src/engine/wgpu/instance.cpp
    src/engine/wgpu/adapter.cpp
    src/engine/wgpu/surface.cpp
)

set(HEADERS
    src/engine/core/glfw_context.h
    src/engine/core/window.h
    src/engine/wgpu/instance.h
    src/engine/wgpu/adapter.h
    src/engine/wgpu/surface.h
)

add_executable(WebGPU-Engine ${SOURCES} ${HEADERS})

target_compile_definitions(WebGPU-Engine PUBLIC GLFW_INCLUDE_NONE)

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/glfw/include)
target_link_libraries(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/glfw/lib/glfw3.lib)

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/webgpu/include)
target_link_libraries(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/webgpu/lib/webgpu_dawn.lib)

add_custom_command(TARGET WebGPU-Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vnd/webgpu/bin/webgpu_dawn.dll"
        $<TARGET_FILE_DIR:WebGPU-Engine>
)
