#include <iostream>

#include "engine/core/glfw_context.h"
#include "engine/core/window.h"
#include "engine/wgpu/adapter.h"
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
      .label = WGPUStringViewInit("Device"),
      .requiredFeatureCount = 0,
      .requiredFeatures = nullptr,
      .defaultQueue =
          {
              .nextInChain = nullptr,
              .label = WGPUStringViewInit("Default Queue"),
          },
  };
  auto device = engine::wgpu::Device(adapter, deviceDescriptor);

  auto queue = engine::wgpu::Queue::FromDevice(device);

  auto glfwContext = engine::core::GLFWContext();
  auto window = engine::core::Window(glfwContext, 1280, 720, "WebGPU Engine");

  auto surface = engine::wgpu::Surface(instance, window);

  WGPUCommandEncoderDescriptor encoderDesc = {};
  encoderDesc.nextInChain = nullptr;
  encoderDesc.label = WGPUStringViewInit("Command Encoder");
  WGPUCommandEncoder encoder =
      wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

  wgpuCommandEncoderInsertDebugMarker(encoder, WGPUStringView("First"));
  wgpuCommandEncoderInsertDebugMarker(encoder, WGPUStringView("Second"));

  WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
  cmdBufferDescriptor.nextInChain = nullptr;
  cmdBufferDescriptor.label = WGPUStringView("Command buffer");
  WGPUCommandBuffer command =
      wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);
  wgpuCommandEncoderRelease(encoder);  // release encoder after it's finished

  // Finally submit the command queue
  std::cout << "Submitting command..." << std::endl;
  wgpuQueueSubmit(queue, 1, &command);
  wgpuCommandBufferRelease(command);
  std::cout << "Command submitted." << std::endl;

  while (!window.ShouldClose())
  {
    glfwContext.PollEvents();
  }

  return 0;
}
