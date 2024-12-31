#ifndef FL_RENDER_PIPELINE_HEADER_DEFINE
#define FL_RENDER_PIPELINE_HEADER_DEFINE
#include <stdbool.h>
#include <webgpu/webgpu.h>

#include "faceless/wgpu/buffer.h"
#include "faceless/wgpu/device.h"

typedef struct
{
  const FL_Buffer* buffer;
  uint32_t binding;
  uint64_t stride;
  bool hasDynamicOffset;
} FL_RenderPipelineUniform;

typedef struct
{
  const char* shader;
  uint64_t vertexStride;
  size_t numVertexAttributes;
  const WGPUVertexAttribute* vertexAttributes;
  const FL_RenderPipelineUniform* vertexUniform;
  const FL_RenderPipelineUniform* fragmentUniform;
  WGPUPrimitiveTopology topology;
  WGPUCullMode cullMode;
  WGPUTextureFormat format;
  bool useBlending;
} FL_RenderPipelineDescriptor;

typedef struct
{
  WGPURenderPipeline handle;
  WGPUBindGroup bindGroup;
} FL_RenderPipeline;

FL_RenderPipeline FL_RenderPipelineCreate(
  const FL_Device* device,
  const FL_RenderPipelineDescriptor* descriptor
);
void FL_RenderPipelineRelease(FL_RenderPipeline* pipeline);

#endif
