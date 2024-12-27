#include <iostream>

#include "engine/core/glfw_context.h"
#include "engine/core/window.h"
#include "engine/wgpu/adapter.h"
#include "engine/wgpu/instance.h"
#include "engine/wgpu/surface.h"

int main()
{
  auto instance = engine::wgpu::Instance();
  auto adapter = engine::wgpu::Adapter(instance);

  WGPUAdapterInfo info{};
  wgpuAdapterGetInfo(adapter, &info);

  std::cout << "Vendor ID: " << info.vendorID << std::endl;

  auto glfwContext = engine::core::GLFWContext();
  auto window = engine::core::Window(glfwContext, 1280, 720, "WebGPU Engine");

  while (!window.ShouldClose())
  {
    glfwContext.PollEvents();
  }

  return 0;
}
