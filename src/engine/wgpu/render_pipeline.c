#include "render_pipeline.h"

FL_RenderPipeline FL_RenderPipelineCreate(
  const FL_Device* device, const WGPURenderPipelineDescriptor* descriptor)
{
  WGPURenderPipeline handle =
    wgpuDeviceCreateRenderPipeline(device->handle, descriptor);

  return (FL_RenderPipeline){.handle = handle};
}

void FL_RenderPipelineRelease(FL_RenderPipeline* pipeline)
{
  wgpuRenderPipelineRelease(pipeline->handle);
}
