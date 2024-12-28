#pragma once

#include <webgpu/webgpu.h>

#include "engine/core/window.h"
#include "engine/wgpu/device.h"
#include "engine/wgpu/instance.h"

namespace engine::wgpu
{
class Surface
{
 public:
  explicit Surface(const Instance& instance, const Device& device,
                   const engine::core::Window& window);
  ~Surface();

  WGPUTextureView GetNextTextureView() const;

  operator WGPUSurface() const
  {
    return _handle;
  }

 private:
  WGPUSurface _handle;
};
}  // namespace engine::wgpu
