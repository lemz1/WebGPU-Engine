#include <iostream>

#include "engine/core/glfw_context.h"
#include "engine/core/window.h"
#include "engine/wgpu/adapter.h"
#include "engine/wgpu/command_buffer.h"
#include "engine/wgpu/command_encoder.h"
#include "engine/wgpu/device.h"
#include "engine/wgpu/instance.h"
#include "engine/wgpu/queue.h"
#include "engine/wgpu/render_pass_encoder.h"
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

  auto surface = engine::wgpu::Surface(instance, device, window);

  while (!window.ShouldClose())
  {
    glfwContext.PollEvents();

    auto textureView = surface.GetNextTextureView();

    auto encoder = engine::wgpu::CommandEncoder(device);

    WGPURenderPassDescriptor renderPassDesc = {};
    renderPassDesc.nextInChain = nullptr;

    WGPURenderPassColorAttachment renderPassColorAttachment = {};
    renderPassColorAttachment.view = textureView;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
    renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
    renderPassColorAttachment.clearValue = WGPUColor{0.1, 0.1, 0.1, 1.0};
    renderPassColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment;
    renderPassDesc.depthStencilAttachment = nullptr;
    renderPassDesc.timestampWrites = nullptr;

    auto renderPass = engine::wgpu::RenderPassEncoder(encoder, &renderPassDesc);
    renderPass.End();

    auto command = engine::wgpu::CommandBuffer(encoder);

    queue.Submit({command});

#ifndef __EMSCRIPTEN__
    surface.Present();
#endif

    device.Tick();
  }

  return 0;
}
