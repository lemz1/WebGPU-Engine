#pragma once

#include <webgpu/webgpu.h>

#include <vector>

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
