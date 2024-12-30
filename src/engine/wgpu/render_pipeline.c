#include "render_pipeline.h"

#include <stdio.h>

FL_RenderPipeline FL_RenderPipelineCreate(
  const FL_Device* device, const WGPURenderPipelineDescriptor* descriptor)
{
  WGPURenderPipeline handle =
    wgpuDeviceCreateRenderPipeline(device->handle, descriptor);
  if (!handle)
  {
    perror("[WebGPU] Could not create Render Pipeline\n");
    return (FL_RenderPipeline){0};
  }

  return (FL_RenderPipeline){.handle = handle};
}

void FL_RenderPipelineRelease(FL_RenderPipeline* pipeline)
{
  wgpuRenderPipelineRelease(pipeline->handle);
}
