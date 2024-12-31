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
    src/faceless/core/window.c
    src/faceless/wgpu/instance.c
    src/faceless/wgpu/adapter.c
    src/faceless/wgpu/surface.c
    src/faceless/wgpu/device.c
    src/faceless/wgpu/queue.c
    src/faceless/wgpu/command_buffer.c
    src/faceless/wgpu/command_encoder.c
    src/faceless/wgpu/texture_view.c
    src/faceless/wgpu/render_pass_encoder.c
    src/faceless/wgpu/render_pipeline.c
    src/faceless/wgpu/shader_module.c
    src/faceless/wgpu/buffer.c

    vnd/glfw3webgpu/glfw3webgpu.c
)

set(HEADERS
    src/faceless/core/window.h
    src/faceless/wgpu/instance.h
    src/faceless/wgpu/adapter.h
    src/faceless/wgpu/surface.h
    src/faceless/wgpu/device.h
    src/faceless/wgpu/queue.h
    src/faceless/wgpu/command_buffer.h
    src/faceless/wgpu/command_encoder.h
    src/faceless/wgpu/texture_view.h
    src/faceless/wgpu/render_pass_encoder.h
    src/faceless/wgpu/render_pipeline.h
    src/faceless/wgpu/shader_module.h
    src/faceless/wgpu/buffer.h
    src/faceless/wgpu/util.h
    src/faceless.h

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
