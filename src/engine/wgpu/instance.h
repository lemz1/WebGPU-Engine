#pragma once

#include <webgpu/webgpu.h>

#include <vector>

namespace engine::wgpu
{
class Instance
{
 public:
  explicit Instance();
  ~Instance();

  void WaitAny(std::vector<WGPUFutureWaitInfo>& futures) const;

  operator WGPUInstance() const
  {
    return _handle;
  }

 private:
  WGPUInstance _handle;
};
}  // namespace engine::wgpu
