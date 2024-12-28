#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/command_encoder.h"

namespace engine::wgpu
{
class CommandBuffer
{
 public:
  explicit CommandBuffer(WGPUCommandBuffer handle) : _handle(handle)
  {
  }
  explicit CommandBuffer(const CommandEncoder& encoder);
  ~CommandBuffer();

  WGPUCommandBuffer GetHandle() const
  {
    return _handle;
  }

  operator WGPUCommandBuffer() const
  {
    return _handle;
  }

 private:
  WGPUCommandBuffer _handle;
};
}  // namespace engine::wgpu
