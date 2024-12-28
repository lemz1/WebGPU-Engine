#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/util.h"

namespace engine::wgpu
{
class TextureView
{
 public:
  explicit TextureView(WGPUTextureView handle) : _handle(handle)
  {
  }
  ~TextureView();

  WGPUTextureView GetHandle() const
  {
    return _handle;
  }

  operator WGPUTextureView() const
  {
    return _handle;
  }

 private:
  WGPUTextureView _handle;
};
}  // namespace engine::wgpu
