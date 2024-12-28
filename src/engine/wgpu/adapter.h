#pragma once

#include <webgpu/webgpu.h>

#include "instance.h"

namespace engine::wgpu
{
class Adapter
{
 public:
  explicit Adapter(WGPUAdapter handle) : _handle(handle)
  {
  }
  explicit Adapter(const Instance& instance);
  ~Adapter();

  WGPUAdapter GetHandle() const
  {
    return _handle;
  }

  operator WGPUAdapter() const
  {
    return _handle;
  }

 private:
  WGPUAdapter _handle;
};
}  // namespace engine::wgpu
