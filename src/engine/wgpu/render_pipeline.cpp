#include "render_pipeline.h"

namespace engine::wgpu
{
RenderPipeline::RenderPipeline(const Device& device,
                               const WGPURenderPipelineDescriptor* descriptor)
{
  _handle = wgpuDeviceCreateRenderPipeline(device, descriptor);
}

RenderPipeline::~RenderPipeline()
{
  wgpuRenderPipelineRelease(_handle);
}
}  // namespace engine::wgpu
