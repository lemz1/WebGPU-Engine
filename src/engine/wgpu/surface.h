#pragma once

#include <webgpu/webgpu.h>

#include "engine/core/window.h"
#include "engine/wgpu/device.h"
#include "engine/wgpu/instance.h"
#include "engine/wgpu/texture_view.h"

namespace engine::wgpu
{
class Surface
{
 public:
  explicit Surface(WGPUSurface handle) : _handle(handle)
  {
  }
  explicit Surface(const Instance& instance, const Device& device,
                   const engine::core::Window& window);
  ~Surface();

  TextureView GetNextTextureView() const;

  void Present() const;

  WGPUSurface GetHandle() const
  {
    return _handle;
  }

  operator WGPUSurface() const
  {
    return _handle;
  }

 private:
  WGPUSurface _handle;
};
}  // namespace engine::wgpu
