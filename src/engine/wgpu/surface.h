#pragma once

#include <webgpu/webgpu.h>

#include "instance.h"

namespace engine::wgpu
{
class Surface
{
 public:
  explicit Surface(const Instance& instance);
  ~Surface();

  operator WGPUSurface() const
  {
    return _handle;
  }

 private:
  WGPUSurface _handle;
};
}  // namespace engine::wgpu
