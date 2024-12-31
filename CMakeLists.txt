cmake_minimum_required(VERSION 3.10)

project(WebGPU-Engine)

set(CMAKE_C_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED True)

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
    src/main.c
    src/engine/core/window.c
    src/engine/wgpu/instance.c
    src/engine/wgpu/adapter.c
    src/engine/wgpu/surface.c
    src/engine/wgpu/device.c
    src/engine/wgpu/queue.c
    src/engine/wgpu/command_buffer.c
    src/engine/wgpu/command_encoder.c
    src/engine/wgpu/texture_view.c
    src/engine/wgpu/render_pass_encoder.c
    src/engine/wgpu/render_pipeline.c
    src/engine/wgpu/shader_module.c
    src/engine/wgpu/buffer.c
    src/engine/wgpu/bind_group.c

    vnd/glfw3webgpu/glfw3webgpu.c
)

set(HEADERS
    src/engine/core/window.h
    src/engine/wgpu/instance.h
    src/engine/wgpu/adapter.h
    src/engine/wgpu/surface.h
    src/engine/wgpu/device.h
    src/engine/wgpu/queue.h
    src/engine/wgpu/command_buffer.h
    src/engine/wgpu/command_encoder.h
    src/engine/wgpu/texture_view.h
    src/engine/wgpu/render_pass_encoder.h
    src/engine/wgpu/render_pipeline.h
    src/engine/wgpu/shader_module.h
    src/engine/wgpu/buffer.h
    src/engine/wgpu/bind_group.h
    src/engine/wgpu/util.h

    vnd/glfw3webgpu/glfw3webgpu.h
)

add_executable(WebGPU-Engine ${SOURCES} ${HEADERS})

target_compile_definitions(WebGPU-Engine PUBLIC GLFW_INCLUDE_NONE)

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/glfw/include)
target_link_libraries(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/glfw/lib/glfw3.lib)

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/webgpu/include)
target_link_libraries(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/webgpu/lib/webgpu_dawn.lib)

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/glfw3webgpu)

add_custom_command(TARGET WebGPU-Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vnd/webgpu/bin/webgpu_dawn.dll"
        $<TARGET_FILE_DIR:WebGPU-Engine>
)
