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
}  // namespace engine::wgpu
