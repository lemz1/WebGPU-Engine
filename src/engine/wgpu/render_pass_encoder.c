#include "render_pass_encoder.h"

FL_RenderPassEncoder FL_RenderPassEncoderCreate(WGPURenderPassEncoder handle)
{
  return (FL_RenderPassEncoder){.handle = handle};
}

void FL_RenderPassEncoderRelease(FL_RenderPassEncoder* renderPass)
{
  wgpuRenderPassEncoderRelease(renderPass->handle);
}

void FL_RenderPassEncoderEnd(const FL_RenderPassEncoder* renderPass)
{
  wgpuRenderPassEncoderEnd(renderPass->handle);
}

void FL_RenderPassEncoderSetPipeline(const FL_RenderPassEncoder* renderPass,
                                     const FL_RenderPipeline* pipeline)
{
  wgpuRenderPassEncoderSetPipeline(renderPass->handle, pipeline->handle);
}

void FL_RenderPassEncoderSetVertexBuffer(const FL_RenderPassEncoder* renderPass,
                                         const FL_Buffer* buffer, uint32_t slot,
                                         uint64_t size, uint64_t offset)
{
  wgpuRenderPassEncoderSetVertexBuffer(renderPass->handle, slot, buffer->handle,
                                       offset, size);
}

void FL_RenderPassEncoderSetIndexBuffer(const FL_RenderPassEncoder* renderPass,
                                        const FL_Buffer* buffer, uint64_t size,
                                        WGPUIndexFormat format, uint64_t offset)
{
  wgpuRenderPassEncoderSetIndexBuffer(renderPass->handle, buffer->handle,
                                      format, offset, size);
}

void FL_RenderPassEncoderDraw(const FL_RenderPassEncoder* renderPass,
                              uint32_t vertexCount, uint32_t instanceCount,
                              uint32_t firstVertex, uint32_t firstInstance)
{
  wgpuRenderPassEncoderDraw(renderPass->handle, vertexCount, instanceCount,
                            firstVertex, firstInstance);
}
void FL_RenderPassEncoderDrawIndexed(const FL_RenderPassEncoder* renderPass,
                                     uint32_t indexCount,
                                     uint32_t instanceCount,
                                     uint32_t firstIndex,
                                     uint32_t firstInstance)
{
  wgpuRenderPassEncoderDrawIndexed(renderPass->handle, indexCount,
                                   instanceCount, firstIndex, 0, firstInstance);
}
