#pragma once

#include <webgpu/webgpu.h>

#include "engine/core/window.h"
#include "engine/wgpu/instance.h"

namespace engine::wgpu
{
class Surface
{
 public:
  explicit Surface(const Instance& instance,
                   const engine::core::Window& window);
  ~Surface();

  operator WGPUSurface() const
  {
    return _handle;
  }

 private:
  WGPUSurface _handle;
};
}  // namespace engine::wgpu
