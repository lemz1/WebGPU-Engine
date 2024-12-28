#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/device.h"

namespace engine::wgpu
{
class RenderPipeline
{
 public:
  explicit RenderPipeline(WGPURenderPipeline handle) : _handle(handle)
  {
  }
  explicit RenderPipeline(const Device& device,
                          const WGPURenderPipelineDescriptor* descriptor);
  ~RenderPipeline();

  WGPURenderPipeline GetHandle() const
  {
    return _handle;
  }

  operator WGPURenderPipeline() const
  {
    return _handle;
  }

 private:
  WGPURenderPipeline _handle;
};
}  // namespace engine::wgpu
