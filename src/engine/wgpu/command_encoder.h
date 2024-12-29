#pragma once

#include <webgpu/webgpu.h>

#include <string>

#include "engine/wgpu/device.h"

namespace engine::wgpu
{
class CommandBuffer;
class RenderPassEncoder;

class CommandEncoder
{
 public:
  explicit CommandEncoder(WGPUCommandEncoder handle) : _handle(handle)
  {
  }
  explicit CommandEncoder(const Device& device);
  ~CommandEncoder();

  void InsertDebugMarker(std::string_view label) const;

  CommandBuffer Finish() const;

  RenderPassEncoder BeginRenderPass(
      const WGPURenderPassDescriptor* descriptor) const;

  WGPUCommandEncoder GetHandle() const
  {
    return _handle;
  }

  operator WGPUCommandEncoder() const
  {
    return _handle;
  }

 private:
  WGPUCommandEncoder _handle;
};
}  // namespace engine::wgpu
