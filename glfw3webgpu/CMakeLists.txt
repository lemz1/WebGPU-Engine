# This is only meant to be included as a subdirectory in another project.
# It assumes that targets 'glfw' and 'webgpu' exist.
# Look at examples/CMakeLists.txt to see how to use it in a project.

# The glfw3webgpu target
add_library(glfw3webgpu STATIC glfw3webgpu.c)
target_include_directories(glfw3webgpu PUBLIC .)

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/glfw/include)
target_link_libraries(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/glfw/lib/glfw3.lib)

target_include_directories(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/webgpu/include)
target_link_libraries(WebGPU-Engine PUBLIC ${CMAKE_SOURCE_DIR}/vnd/webgpu/lib/webgpu_dawn.lib)

if (APPLE)
  target_compile_options(glfw3webgpu PRIVATE -x objective-c)
  target_link_libraries(glfw3webgpu PRIVATE "-framework Cocoa" "-framework CoreVideo" "-framework IOKit" "-framework QuartzCore")
endif ()
