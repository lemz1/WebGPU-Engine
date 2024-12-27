#include <iostream>

#include "engine/core/glfw_context.h"
#include "engine/core/window.h"
#include "engine/wgpu/adapter.h"
#include "engine/wgpu/command_buffer.h"
#include "engine/wgpu/command_encoder.h"
#include "engine/wgpu/device.h"
#include "engine/wgpu/instance.h"
#include "engine/wgpu/queue.h"
#include "engine/wgpu/surface.h"
#include "engine/wgpu/util.h"

int main()
{
  auto instance = engine::wgpu::Instance();
  auto adapter = engine::wgpu::Adapter(instance);

  WGPUDeviceDescriptor deviceDescriptor{
      .label = StrToWGPU("Device"),
      .requiredFeatureCount = 0,
      .requiredFeatures = nullptr,
      .defaultQueue =
          {
              .nextInChain = nullptr,
              .label = StrToWGPU("Default Queue"),
          },
  };
  auto device = engine::wgpu::Device(adapter, deviceDescriptor);

  auto queue = engine::wgpu::Queue::FromDevice(device);

  auto glfwContext = engine::core::GLFWContext();
  auto window = engine::core::Window(glfwContext, 1280, 720, "WebGPU Engine");

  auto surface = engine::wgpu::Surface(instance, window);

  {
    auto encoder = engine::wgpu::CommandEncoder(device);

    encoder.InsertDebugMarker("First");
    encoder.InsertDebugMarker("Second");

    auto command = engine::wgpu::CommandBuffer(encoder);

    std::cout << "Submitting command..." << std::endl;
    WGPUFuture future = queue.Submit({command});
    std::cout << "Command submitted." << std::endl;

    std::vector<WGPUFutureWaitInfo> waitInfos = {{future}};
    instance.WaitAny(waitInfos);
  }

  while (!window.ShouldClose())
  {
    glfwContext.PollEvents();
  }

  return 0;
}
