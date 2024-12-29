#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/buffer.h"
#include "engine/wgpu/command_encoder.h"
#include "engine/wgpu/render_pipeline.h"

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

  void SetPipeline(const RenderPipeline& pipeline) const;

  void SetVertexBuffer(const Buffer& buffer, uint32_t slot, uint64_t size,
                       uint64_t offset = 0) const;

  void Draw(uint32_t vertexCount, uint32_t instanceCount = 1,
            uint32_t firstVertex = 0, uint32_t firstInstance = 0) const;

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
