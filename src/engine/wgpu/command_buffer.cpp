#include "command_buffer.h"

#include <iostream>

#include "engine/wgpu/util.h"

namespace engine::wgpu
{
CommandBuffer::CommandBuffer(const CommandEncoder& encoder)
{
  WGPUCommandBufferDescriptor descriptor{
      .nextInChain = nullptr,
      .label = StrToWGPU("Command Buffer"),
  };
  _handle = wgpuCommandEncoderFinish(encoder, &descriptor);
  if (!_handle)
  {
    std::cerr << "[WebGPU] Could not create Command Buffer" << std::endl;
  }
}

CommandBuffer::~CommandBuffer()
{
  wgpuCommandBufferRelease(_handle);
}
}  // namespace engine::wgpu
