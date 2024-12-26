cmake_minimum_required(VERSION 3.10)

project(WebGPU-Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
  src/main.cpp
)

set(HEADERS
)

add_executable(WebGPU-Engine ${SOURCES} ${HEADERS})

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/webgpu/include)
target_link_libraries(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/webgpu/lib/webgpu_dawn.lib)
