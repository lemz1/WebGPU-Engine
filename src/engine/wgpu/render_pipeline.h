#ifndef FL_RENDER_PIPELINE_HEADER_DEFINE
#define FL_RENDER_PIPELINE_HEADER_DEFINE
#include <webgpu/webgpu.h>

#include "engine/wgpu/device.h"

typedef struct
{
  WGPURenderPipeline handle;
} FL_RenderPipeline;

FL_RenderPipeline FL_RenderPipelineCreate(
  const FL_Device* device, const WGPURenderPipelineDescriptor* descriptor);
void FL_RenderPipelineRelease(FL_RenderPipeline* pipeline);

#endif
