#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/adapter.h"

namespace engine::wgpu
{
class Device
{
 public:
  explicit Device(const Adapter& adapter,
                  const WGPUDeviceDescriptor* descriptor);
  ~Device();

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
};
}  // namespace engine::wgpu
