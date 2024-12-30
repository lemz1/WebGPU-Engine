#include "command_encoder.h"

#include <stdio.h>

#include "engine/wgpu/util.h"

FL_CommandEncoder FL_CommandEncoderCreate(const FL_Device* device)
{
  WGPUCommandEncoderDescriptor descriptor = {
    .nextInChain = NULL,
    .label = FL_StrToWGPU("Command Encoder"),
  };
  WGPUCommandEncoder handle =
    wgpuDeviceCreateCommandEncoder(device->handle, &descriptor);
  if (!handle)
  {
    perror("[WebGPU] Could not create Command Encoder\n");
    return (FL_CommandEncoder){0};
  }

  return (FL_CommandEncoder){.handle = handle};
}

void FL_CommandEncoderRelease(FL_CommandEncoder* encoder)
{
  wgpuCommandEncoderRelease(encoder->handle);
}

FL_CommandBuffer FL_CommandEncoderFinish(const FL_CommandEncoder* encoder)
{
  WGPUCommandBufferDescriptor descriptor = {
    .nextInChain = NULL,
    .label = FL_StrToWGPU("Command Buffer"),
  };
  WGPUCommandBuffer handle =
    wgpuCommandEncoderFinish(encoder->handle, &descriptor);

  return FL_CommandBufferCreate(handle);
}

FL_RenderPassEncoder FL_CommandEncoderBeginRenderPass(
  const FL_CommandEncoder* encoder, const WGPURenderPassDescriptor* descriptor)
{
  WGPURenderPassEncoder handle =
    wgpuCommandEncoderBeginRenderPass(encoder->handle, descriptor);

  return FL_RenderPassEncoderCreate(handle);
}
