#pragma once

#include <webgpu/webgpu.h>

#include <vector>

namespace engine::wgpu
{
class Instance
{
 public:
  explicit Instance(WGPUInstance handle) : _handle(handle)
  {
  }
  explicit Instance();
  ~Instance();

  void WaitAny(std::vector<WGPUFutureWaitInfo>& futures) const;

  WGPUInstance GetHandle() const
  {
    return _handle;
  }

  operator WGPUInstance() const
  {
    return _handle;
  }

 private:
  WGPUInstance _handle;
};
}  // namespace engine::wgpu
