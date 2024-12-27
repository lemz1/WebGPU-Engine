#include "command_encoder.h"

#include <iostream>

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
}  // namespace engine::wgpu
