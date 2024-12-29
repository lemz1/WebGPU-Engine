#include "command_encoder.h"

#include <iostream>

#include "engine/wgpu/command_buffer.h"
#include "engine/wgpu/render_pass_encoder.h"
#include "engine/wgpu/util.h"

namespace engine::wgpu
{
CommandEncoder::CommandEncoder(const Device& device)
{
  WGPUCommandEncoderDescriptor descriptor{
      .nextInChain = nullptr,
      .label = StrToWGPU("Command Encoder"),
  };
  _handle = wgpuDeviceCreateCommandEncoder(device, &descriptor);
  if (!_handle)
  {
    std::cerr << "[WebGPU] Could not create Command Encoder" << std::endl;
  }
}

CommandEncoder::~CommandEncoder()
{
  wgpuCommandEncoderRelease(_handle);
}

void CommandEncoder::InsertDebugMarker(std::string_view label) const
{
  wgpuCommandEncoderInsertDebugMarker(_handle, ViewToWGPU(label));
}

CommandBuffer CommandEncoder::Finish() const
{
  return CommandBuffer(*this);
}

RenderPassEncoder CommandEncoder::BeginRenderPass(
    const WGPURenderPassDescriptor* descriptor) const
{
  return RenderPassEncoder(*this, descriptor);
}
}  // namespace engine::wgpu
