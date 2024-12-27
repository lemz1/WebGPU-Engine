#pragma once

#include <webgpu/webgpu.h>

#include <vector>

#include "engine/wgpu/device.h"

namespace engine::wgpu
{
class Queue
{
 public:
  static Queue FromDevice(const Device& device);
  ~Queue();

  WGPUFuture Submit(const std::vector<WGPUCommandBuffer>& commands) const;

  WGPUQueue GetHandle() const
  {
    return _handle;
  }

  operator WGPUQueue() const
  {
    return _handle;
  }

 private:
  explicit Queue(WGPUQueue handle);

 private:
  WGPUQueue _handle;
};
}  // namespace engine::wgpu
