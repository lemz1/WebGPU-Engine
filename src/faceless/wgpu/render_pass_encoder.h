#ifndef FL_RENDER_PASS_ENCODER_HEADER_DEFINE
#define FL_RENDER_PASS_ENCODER_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "faceless/wgpu/buffer.h"
#include "faceless/wgpu/render_pipeline.h"

typedef struct
{
  WGPURenderPassEncoder handle;
} FL_RenderPassEncoder;

FL_RenderPassEncoder FL_RenderPassEncoderCreate(WGPURenderPassEncoder handle);
void FL_RenderPassEncoderRelease(FL_RenderPassEncoder* renderPass);

void FL_RenderPassEncoderEnd(const FL_RenderPassEncoder* renderPass);

void FL_RenderPassEncoderSetPipeline(
  const FL_RenderPassEncoder* renderPass,
  const FL_RenderPipeline* pipeline
);

void FL_RenderPassEncoderSetVertexBuffer(
  const FL_RenderPassEncoder* renderPass,
  const FL_Buffer* buffer,
  uint32_t slot,
  uint64_t size,
  uint64_t offset
);

void FL_RenderPassEncoderSetIndexBuffer(
  const FL_RenderPassEncoder* renderPass,
  const FL_Buffer* buffer,
  uint64_t size,
  WGPUIndexFormat format,
  uint64_t offset
);

void FL_RenderPassEncoderDraw(
  const FL_RenderPassEncoder* renderPass,
  uint32_t vertexCount,
  uint32_t instanceCount,
  uint32_t firstVertex,
  uint32_t firstInstance
);

void FL_RenderPassEncoderDrawIndexed(
  const FL_RenderPassEncoder* renderPass,
  uint32_t indexCount,
  uint32_t instanceCount,
  uint32_t firstIndex,
  uint32_t firstInstance
);

void FL_RenderPassEncoderSetBindGroup(
  const FL_RenderPassEncoder* renderPass,
  uint32_t groupIndex,
  WGPUBindGroup group,
  size_t numOffsets,
  const uint32_t* offsets
);

#endif
