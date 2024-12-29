#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/adapter.h"

namespace engine::wgpu
{
class Queue;

class Device
{
 public:
  explicit Device(WGPUDevice handle) : _handle(handle)
  {
  }
  explicit Device(const Adapter& adapter);
  ~Device();

  void Tick() const;

  Queue GetQueue() const;

  WGPUDevice GetHandle() const
  {
    return _handle;
  }

  operator WGPUDevice() const
  {
    return _handle;
  }

 private:
  WGPUDevice _handle;
  WGPURequiredLimits _requiredLimits;
};
}  // namespace engine::wgpu
