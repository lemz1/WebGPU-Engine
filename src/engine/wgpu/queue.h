#pragma once

#include <webgpu/webgpu.h>

#include <vector>

#include "engine/wgpu/buffer.h"
#include "engine/wgpu/device.h"

namespace engine::wgpu
{
class Queue
{
 public:
  explicit Queue(WGPUQueue handle) : _handle(handle)
  {
  }
  static Queue FromDevice(const Device& device);
  ~Queue();

  void Submit(const std::vector<WGPUCommandBuffer>& commands) const;
  WGPUFuture SubmitFuture(const std::vector<WGPUCommandBuffer>& commands) const;

  void WriteBuffer(const Buffer& buffer, uint64_t size, const void* data,
                   uint64_t offset = 0) const;

  WGPUQueue GetHandle() const
  {
    return _handle;
  }

  operator WGPUQueue() const
  {
    return _handle;
  }

 private:
  WGPUQueue _handle;
};
}  // namespace engine::wgpu
