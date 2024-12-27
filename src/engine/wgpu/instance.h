#pragma once

#include <webgpu/webgpu.h>

namespace engine::wgpu
{
class Instance
{
 public:
  explicit Instance();
  ~Instance();

  operator WGPUInstance() const
  {
    return _handle;
  }

 private:
  WGPUInstance _handle;
};
}  // namespace engine::wgpu
