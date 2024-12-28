#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/command_encoder.h"

namespace engine::wgpu
{
class RenderPassEncoder
{
 public:
  explicit RenderPassEncoder(WGPURenderPassEncoder handle) : _handle(handle)
  {
  }
  explicit RenderPassEncoder(const CommandEncoder& encoder,
                             const WGPURenderPassDescriptor* descriptor);
  ~RenderPassEncoder();

  void End() const;

  WGPURenderPassEncoder GetHandle() const
  {
    return _handle;
  }

  operator WGPURenderPassEncoder() const
  {
    return _handle;
  }

 private:
  WGPURenderPassEncoder _handle;
};
}  // namespace engine::wgpu
