#include "render_pass_encoder.h"

namespace engine::wgpu
{
RenderPassEncoder::RenderPassEncoder(const CommandEncoder& encoder,
                                     const WGPURenderPassDescriptor* descriptor)
{
  _handle = wgpuCommandEncoderBeginRenderPass(encoder, descriptor);
}

RenderPassEncoder::~RenderPassEncoder()
{
  wgpuRenderPassEncoderRelease(_handle);
}

void RenderPassEncoder::End() const
{
  wgpuRenderPassEncoderEnd(_handle);
}

void RenderPassEncoder::SetPipeline(const RenderPipeline& pipeline) const
{
  wgpuRenderPassEncoderSetPipeline(_handle, pipeline);
}

void RenderPassEncoder::Draw(uint32_t vertexCount, uint32_t instanceCount,
                             uint32_t firstVertex, uint32_t firstInstance) const
{
  wgpuRenderPassEncoderDraw(_handle, vertexCount, instanceCount, firstVertex,
                            firstInstance);
}
}  // namespace engine::wgpu
