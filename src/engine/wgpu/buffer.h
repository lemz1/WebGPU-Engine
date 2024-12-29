#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/device.h"

namespace engine::wgpu
{
class Buffer
{
 public:
  explicit Buffer(WGPUBuffer handle) : _handle(handle)
  {
  }
  explicit Buffer(const Device& device, uint64_t size, WGPUBufferUsage usage);
  ~Buffer();

  WGPUBuffer GetHandle() const
  {
    return _handle;
  }

  operator WGPUBuffer() const
  {
    return _handle;
  }

  uint64_t GetSize() const
  {
    return _size;
  }

 private:
  WGPUBuffer _handle;
  uint64_t _size;
};
}  // namespace engine::wgpu
